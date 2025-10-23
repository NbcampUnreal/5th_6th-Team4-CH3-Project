#include "Item/MiscItem.h"
#include "Item/ItemDataRow.h"
#include "Character/AttributeComponent.h"

void UMiscItem::AddAttributeModifiers(UAttributeComponent* AttributeComponent, EItemRarity)
{
	if (IsValid(AttributeComponent) == false)
		return;

	const FMiscItemDataRow* MiscData = (FMiscItemDataRow*)Data;
	if (MiscData == nullptr)
		return;

	for (const auto& [AttributeTag, Modifier] : MiscData->AttributeModifiers)
	{
		float AdditionalValue = Modifier.Value;
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

void UMiscItem::RemoveAttributeModifiers(UAttributeComponent* AttributeComponent)
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