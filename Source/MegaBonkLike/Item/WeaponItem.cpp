#include "Item/WeaponItem.h"
#include "Item/ItemDataRow.h"
#include "Character/AttributeComponent.h"

void UWeaponItem::AddAttributeModifiers(UAttributeComponent*, EItemRarity InRarity)
{
	const FWeaponItemDataRow* WeaponData = (FWeaponItemDataRow*)Data;
	if (WeaponData == nullptr)
		return;

	for (const auto& Entry : WeaponData->WeaponAttributeEntry)
	{
		if (Level == 1)
		{
			WeaponAttributeSet.AddAttribute(Entry.AttributeTag, Entry.BaseValue);
		}
		else
		{
			FAttributeModifier NewModifier;
			NewModifier.Type = Entry.UpgradeModifier.Type;
			NewModifier.Value = Entry.UpgradeModifier.Value * GetRarityMultiplier(InRarity);
			WeaponAttributeSet.AddModifier(Entry.AttributeTag, NewModifier);
		}
	}
}

void UWeaponItem::SetSkillId(int32 InSkillId)
{
    SkillId = InSkillId;
}

void UWeaponItem::AddAttributeChangedCallback(const FGameplayTag& Tag, TWeakObjectPtr<UObject> Instigator, const TFunction<void(const TWeakObjectPtr<UAttribute>)> NewCallback)
{
	return WeaponAttributeSet.AddChangedCallback(Tag, Instigator, NewCallback);
}

void UWeaponItem::RemoveAttributeChangedCallback(const FGameplayTag& Tag, TWeakObjectPtr<UObject> Instigator)
{
	WeaponAttributeSet.RemoveChangedCallback(Tag, Instigator);
}

float UWeaponItem::GetAttributeValue(const FGameplayTag& AttributeTag) const
{
	return WeaponAttributeSet.GetAttributeValue(AttributeTag);
}
