#include "Item/WeaponItem.h"
#include "Item/ItemDataRow.h"
#include "Character/AttributeComponent.h"
#include "Character/SkillComponent.h"
#include "Skill/Skill.h"

void UWeaponItem::Init(AActor* InOwner, const FItemDataRow* InData)
{
	Super::Init(InOwner, InData);

	Level = 1;

	const FWeaponItemDataRow* WeaponData = (FWeaponItemDataRow*)Data;
	if (WeaponData == nullptr)
		return;

	for (const auto& Entry : WeaponData->WeaponAttributeEntry)
	{
		WeaponAttributeSet.AddAttribute(Entry.AttributeTag, Entry.BaseValue);
	}

	if (Owner.IsValid() == false)
		return;

	if (USkillComponent* SkillComponent = Owner->FindComponentByClass<USkillComponent>())
	{
		USkill* CDO = WeaponData->SkillClass->GetDefaultObject<USkill>();
		USkill* NewSkill = DuplicateObject<USkill>(CDO, SkillComponent);
		NewSkill->SetOwnerWeapon(this);
		SkillId = SkillComponent->AddSkill(NewSkill);
	}
}

void UWeaponItem::Upgrade(const FItemUpgradeContext& Context)
{
	++Level;

	const FWeaponItemDataRow* WeaponData = (FWeaponItemDataRow*)Data;
	if (WeaponData == nullptr)
		return;

	for (const auto& [Tag, Modifier] : Context.AttributeChanges)
	{
		WeaponAttributeSet.AddModifier(Tag, Modifier);
	}
}

void UWeaponItem::OnDestroy()
{
	if (Owner.IsValid() == false)
		return;

	if (USkillComponent* SkillComponent = Owner->FindComponentByClass<USkillComponent>())
	{
		SkillComponent->RemoveSkill(SkillId);
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
