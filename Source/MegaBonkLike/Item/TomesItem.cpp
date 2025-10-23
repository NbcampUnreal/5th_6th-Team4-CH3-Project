#include "Item/TomesItem.h"
#include "Item/ItemDataRow.h"
#include "Entity/AttributeComponent.h"

void UTomesItem::AddAttributeModifiers(UAttributeComponent* AttributeComponent, EItemRarity InRarity)
{
	if (IsValid(AttributeComponent) == false)
		return;

	const FTomesItemDataRow* TomesData = (FTomesItemDataRow*)Data;
	if (TomesData == nullptr)
		return;

	for (const auto& [AttributeTag, Modifier] : TomesData->AttributeModifiers)
	{
		float AdditionalValue = Modifier.Value * GetRarityMultiplier(InRarity);
		FAttributeModifier* ThisModifier = Modifiers.Find(AttributeTag);
		if (ThisModifier == nullptr)
		{
			FAttributeModifier NewModifier;
			NewModifier.Type = Modifier.Type;
			NewModifier.Value = 0.0f;
			Modifiers.Add(AttributeTag, NewModifier);

			ThisModifier = Modifiers.Find(AttributeTag);
		}
		ThisModifier->Value += AdditionalValue;

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

void UTomesItem::RemoveAttributeModifiers(UAttributeComponent* AttributeComponent)
{
	if (IsValid(AttributeComponent) == false)
		return;

	for (const auto& [AttributeTag, ModifierId] : ModifierIds)
	{
		AttributeComponent->RemoveModifier(EAttributeSourceType::Player, AttributeTag, ModifierId);
	}
	ModifierIds.Empty();
	Modifiers.Empty();
}
