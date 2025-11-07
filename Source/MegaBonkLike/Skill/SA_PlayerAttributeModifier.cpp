#include "Skill/SA_PlayerAttributeModifier.h"
#include "Character/MBLPlayerCharacter.h"
#include "Character/AttributeComponent.h"
#include "Game/MBLGameState.h"

void USA_PlayerAttributeModifier::Activate(TWeakObjectPtr<AActor> InInstigator)
{
	Super::Activate(InInstigator);

	AMBLPlayerCharacter* Player = Cast<AMBLPlayerCharacter>(Instigator);
	if (IsValid(Player) == false)
		return;

	for (int32 Index = 0; Index < AttributeTriggers.Num(); ++Index)
	{
		switch (AttributeTriggers[Index].TriggerType)
		{
			case EAttributeTriggerType::None:
				ApplyModifier(Index, AttributeTriggers[Index].AttributeModifier);
				break;
			case EAttributeTriggerType::Player_OnDamaged:
				Player->OnDamaged.AddDynamic(this, &ThisClass::HandlePlayerDamaged);
				break;
			case EAttributeTriggerType::Player_OnKillCountOver:
			case EAttributeTriggerType::Player_OnEvery_N_Kills:
				{
					UWorld* World = GetWorld();
					if (IsValid(World) == false)
						break;

					AMBLGameState* GameState = World->GetGameState<AMBLGameState>();
					if (IsValid(GameState) == false)
						break;

					HandleKilled(GameState->GetKills());
					GameState->OnChangedKillCount.AddDynamic(this, &ThisClass::HandleKilled);
				}
				break;
			case EAttributeTriggerType::Player_LowerHP:
			case EAttributeTriggerType::Player_UpperHP:
				{
					HandleHPChanged(Player->GetCurrHP(), Player->GetMaxHP());
					Player->OnHPChanged.AddDynamic(this, &ThisClass::HandleHPChanged);
				}
				break;
			default:
				continue;
		}
	}
}

void USA_PlayerAttributeModifier::Deactivate()
{
	Super::Deactivate();

	for (const auto& [TriggerId, TriggeredModifierInfo] : TriggeredModifierInfos)
	{
		RemoveModifier(TriggerId);
	}
}

void USA_PlayerAttributeModifier::HandlePlayerDamaged(float Damage, AActor* DamageCauser)
{
	for (int32 Index = 0; Index < AttributeTriggers.Num(); ++Index)
	{
		if (AttributeTriggers[Index].TriggerType == EAttributeTriggerType::Player_OnDamaged)
		{
			if (Damage < AttributeTriggers[Index].TriggerValue)
				continue;

			ApplyModifier(Index, AttributeTriggers[Index].AttributeModifier);
		}
	}
}

void USA_PlayerAttributeModifier::HandleKilled(int32 KillCount)
{
	for (int32 Index = 0; Index < AttributeTriggers.Num(); ++Index)
	{
		if (AttributeTriggers[Index].TriggerType == EAttributeTriggerType::Player_OnKillCountOver)
		{
			if (KillCount >= AttributeTriggers[Index].TriggerValue)
			{
				ApplyModifier(Index, AttributeTriggers[Index].AttributeModifier);
			}
		}
		else if (AttributeTriggers[Index].TriggerType == EAttributeTriggerType::Player_OnEvery_N_Kills)
		{
			int32 Multiplier = KillCount / AttributeTriggers[Index].TriggerValue;
			FAttributeModifier Modifier = AttributeTriggers[Index].AttributeModifier;
			Modifier.Value *= Multiplier;
			ApplyModifier(Index, Modifier);
		}
	}
}

void USA_PlayerAttributeModifier::HandleHPChanged(float CurrHP, float MaxHP)
{
	for (int32 Index = 0; Index < AttributeTriggers.Num(); ++Index)
	{
		if (AttributeTriggers[Index].TriggerType != EAttributeTriggerType::Player_LowerHP &&
			AttributeTriggers[Index].TriggerType != EAttributeTriggerType::Player_UpperHP)
			continue;

		float HPRatio = CurrHP / MaxHP;
		bool bTrigger = false;
		switch (AttributeTriggers[Index].TriggerType)
		{
			case EAttributeTriggerType::Player_LowerHP: bTrigger = HPRatio < AttributeTriggers[Index].TriggerValue; break;
			case EAttributeTriggerType::Player_UpperHP: bTrigger = HPRatio >= AttributeTriggers[Index].TriggerValue; break;
			default: break;
		}

		if (bTrigger == true)
		{
			ApplyModifier(Index, AttributeTriggers[Index].AttributeModifier);
		}
		else
		{
			RemoveModifier(Index);
		}
	}
}

FAttributeModifier USA_PlayerAttributeModifier::GetBaseCalculatedModifierInfo(int32 TriggerIndex) const
{
	const FAttributeTrigger& Trigger = AttributeTriggers[TriggerIndex];
	return Trigger.AttributeModifier;
}

void USA_PlayerAttributeModifier::ApplyModifier(int32 TriggerIndex, const FAttributeModifier& CalculatedModifier)
{
	if (Instigator.IsValid() == false)
		return;

	UAttributeComponent* AttributeComponent = Instigator->FindComponentByClass<UAttributeComponent>();
	if (IsValid(AttributeComponent) == false)
		return;

	const FAttributeTrigger& Trigger = AttributeTriggers[TriggerIndex];

	if (TriggeredModifierInfos.Contains(TriggerIndex))
	{
		FTriggeredModifierInfo& ExistInfo = TriggeredModifierInfos[TriggerIndex];
		if (FMath::IsNearlyZero(Trigger.Duration))
		{
			AttributeComponent->ChangeModifier(EAttributeSourceType::Player, ExistInfo.AttributeTag, ExistInfo.ModifierId, CalculatedModifier);
			return;
		}
	}

	RemoveModifier(TriggerIndex);
	FGameplayTag Tag = Trigger.AttributeTag;
	FAttributeModifier Modifier = CalculatedModifier;
	int ModifierId = AttributeComponent->AddModifer(EAttributeSourceType::Player, Tag, Modifier);
	FTriggeredModifierInfo ModifierInfo;
	ModifierInfo.AttributeTag = Tag;
	ModifierInfo.ModifierId = ModifierId;
	TriggeredModifierInfos.Add(TriggerIndex, ModifierInfo);

	UWorld* World = GetWorld();
	if (World == nullptr)
		return;

	FTimerDelegate RemoveModifierDelegate;
	RemoveModifierDelegate.BindUObject(this, &ThisClass::RemoveModifier, TriggerIndex);
	World->GetTimerManager().SetTimer(
		ModifierInfo.DurationTimerHandle,
		RemoveModifierDelegate,
		Trigger.Duration,
		false);
}

void USA_PlayerAttributeModifier::RemoveModifier(int32 TriggerIndex)
{
	if (Instigator.IsValid() == false)
		return;

	UAttributeComponent* AttributeComponent = Instigator->FindComponentByClass<UAttributeComponent>();
	if (IsValid(AttributeComponent) == false)
		return;

	if (TriggeredModifierInfos.Contains(TriggerIndex) == false)
		return;

	FTriggeredModifierInfo& Info = TriggeredModifierInfos[TriggerIndex];
	if (Info.DurationTimerHandle.IsValid() == true)
	{
		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().ClearTimer(Info.DurationTimerHandle);
		}
	}

	FGameplayTag Tag = Info.AttributeTag;
	int32 ModifierId = Info.ModifierId;
	AttributeComponent->RemoveModifier(EAttributeSourceType::Player, Tag, ModifierId);
	TriggeredModifierInfos.Remove(TriggerIndex);
}
