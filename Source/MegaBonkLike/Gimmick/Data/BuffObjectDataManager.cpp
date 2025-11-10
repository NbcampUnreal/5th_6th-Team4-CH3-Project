#include "Gimmick/Data/BuffObjectDataManager.h"
#include "Gimmick/Data/ShrineOption.h"
#include "Character/MBLPlayerCharacter.h"
#include "Character/AttributeComponent.h"

void UBuffObjectDataManager::ApplyShrineBuff(const FShrineOption& SelectedOption)
{
	UAttributeComponent* AttributeComponent = nullptr;
	if (UWorld* World = GetWorld())
		if (APlayerController* PlayerController = World->GetFirstPlayerController())
			if (AMBLPlayerCharacter* Player = Cast<AMBLPlayerCharacter>(PlayerController->GetPawn()))
				AttributeComponent = Player->FindComponentByClass<UAttributeComponent>();

	if (IsValid(AttributeComponent) == false)
		return;

	FAttributeModifier* ThisModifier = Modifiers.Find(SelectedOption.Tag);
	if (ThisModifier == nullptr)
	{
		FAttributeModifier NewModifier;
		NewModifier.Type = SelectedOption.Modifier.Type;
		NewModifier.Value = 0.0f;
		Modifiers.Add(SelectedOption.Tag, NewModifier);

		ThisModifier = Modifiers.Find(SelectedOption.Tag);
	}
	ThisModifier->Value += SelectedOption.Modifier.Value;

	if (int32* Id = ModifierIds.Find(SelectedOption.Tag))
	{
		AttributeComponent->ChangeModifier(EAttributeSourceType::Player, SelectedOption.Tag, *Id, *ThisModifier);
	}
	else
	{
		int32 NewId = AttributeComponent->AddModifer(EAttributeSourceType::Player, SelectedOption.Tag, *ThisModifier);
		ModifierIds.Add(SelectedOption.Tag, NewId);
	}
}