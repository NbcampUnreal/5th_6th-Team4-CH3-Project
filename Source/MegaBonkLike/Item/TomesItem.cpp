#include "Item/TomesItem.h"
#include "Item/ItemDataRow.h"
#include "Character/AttributeComponent.h"

void UTomesItem::Init(AActor* InOwner, const FItemDataRow* InData)
{
	Super::Init(InOwner, InData);

	Level = 1;

	const FTomesItemDataRow* TomesData = (FTomesItemDataRow*)Data;
	if (TomesData == nullptr)
		return;
	
	AddAttributeModifiers(TomesData->AttributeModifiers);
}

void UTomesItem::Upgrade(const FItemUpgradeContext& UpgradeContext)
{
	++Level;
	AddAttributeModifiers(UpgradeContext.AttributeChanges);
}

void UTomesItem::OnDestroy()
{
	RemoveAttributeModifiers();
}

int32 UTomesItem::GetLevel() const
{
	return Level;
}

void UTomesItem::AddAttributeModifiers(const TMap<FGameplayTag, FAttributeModifier>& InModifiers)
{
	UAttributeComponent* AttributeComponent = GetOwnerAttributeComponent();
	if (AttributeComponent == nullptr)
		return;

	for (const auto& [AttributeTag, Modifier] : InModifiers)
	{
		FAttributeModifier* ThisModifier = Modifiers.Find(AttributeTag);
		if (ThisModifier == nullptr)
		{
			FAttributeModifier NewModifier;
			NewModifier.Type = Modifier.Type;
			NewModifier.Value = 0.0f;
			Modifiers.Add(AttributeTag, NewModifier);

			ThisModifier = Modifiers.Find(AttributeTag);
		}
		ThisModifier->Value += Modifier.Value;

		if (const int32* ExistModifierId = ModifierIds.Find(AttributeTag))
		{
			AttributeComponent->ChangeModifier(EAttributeSourceType::Player, AttributeTag, *ExistModifierId, *ThisModifier);
		}
		else
		{
			int32 NewId = AttributeComponent->AddModifer(EAttributeSourceType::Player, AttributeTag, *ThisModifier);
			ModifierIds.Add(AttributeTag, NewId);
		}
	}
}

void UTomesItem::RemoveAttributeModifiers()
{
	UAttributeComponent* AttributeComponent = GetOwnerAttributeComponent();
	if (AttributeComponent == nullptr)
		return;

	for (const auto& [AttributeTag, ModifierId] : ModifierIds)
	{
		AttributeComponent->RemoveModifier(EAttributeSourceType::Player, AttributeTag, ModifierId);
	}
	ModifierIds.Empty();
	Modifiers.Empty();
}

float UTomesItem::GetModifierValue(const FGameplayTag& AttributeTag) const
{
	auto* Modifier = Modifiers.Find(AttributeTag);
	return Modifier != nullptr ? Modifier->Value : 0.0f;
}
