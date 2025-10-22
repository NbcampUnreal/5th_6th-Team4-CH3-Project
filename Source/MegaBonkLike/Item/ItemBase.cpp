#include "Item/ItemBase.h"
#include "Item/ItemDataRow.h"
#include "Entity/AttributeComponent.h"

void UItemBase::InitializeItem(const FItemDataRow* InData)
{
    Data = InData;
}

void UItemBase::Upgrade()
{
    ++Level;
}

void UItemBase::AddAttributeModifiers(UAttributeComponent* AttributeComponent, EItemRarity InRarity)
{
	if (IsValid(AttributeComponent) == false)
		return;

    for (const auto& [AttributeTag, Modifier] : Data->AttributeModifiers)
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

		if (Data->ItemType != EItemType::Weapon)
		{
			FAttribute* Attribute = AttributeComponent->GetAttribute(AttributeTag);
			check(Attribute != nullptr);
			if (const int32* ExistModifierId = ModifierIds.Find(AttributeTag))
			{
				Attribute->ChangeModifier(*ExistModifierId, *ThisModifier);
			}
			else
			{
				int32 NewId = Attribute->AddModifier(*ThisModifier);
				ModifierIds.Add(AttributeTag, NewId);
			}
		}
    }
}

void UItemBase::RemoveAttributeModifiers(UAttributeComponent* AttributeComponent)
{
	if (IsValid(AttributeComponent) == false)
		return;

	for (const auto& [AttributeTag, ModifierId] : ModifierIds)
	{
		if (FAttribute* Attribute = AttributeComponent->GetAttribute(AttributeTag))
		{
			Attribute->RemoveModifier(ModifierId);
		}
	}
	ModifierIds.Empty();
	Modifiers.Empty();
}

void UItemBase::SetSkillId(int32 InSkillId)
{
    SkillId = InSkillId;
}

const FAttributeModifier* UItemBase::GetModifier(const FGameplayTag& AttributeTag) const
{
	return Modifiers.Find(AttributeTag);
}

float UItemBase::GetRarityMultiplier(EItemRarity InRarity)
{
	if (Data->ItemType != EItemType::Misc)
		return 1.0f;

	switch (InRarity)
	{
		case EItemRarity::Common: return 1.0f;
		case EItemRarity::Uncommon: return 1.2f;
		case EItemRarity::Rare: return 1.4f;
		case EItemRarity::Epic: return 1.6f;
		case EItemRarity::Legendary: return 2.0f;
		default: return 1.0f;
	}
}
