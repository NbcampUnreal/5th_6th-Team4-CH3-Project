#include "Skill/WeaponSkillAction.h"
#include "Item/WeaponItem.h"
#include "Attack/AttackData.h"
#include "Attribute/AttributeTags.h"

float UWeaponSkillAction::GetValue(const FGameplayTag& AttributeTag) const
{
    float PlayerAttributeValue = Super::GetValue(AttributeTag);
    float WeaponValue = OwnerWeapon.IsValid() == false ? 1.0f : OwnerWeapon->GetAttributeValue(AttributeTag);
    return PlayerAttributeValue * WeaponValue;
}

void UWeaponSkillAction::SetOwnerWeapon(TWeakObjectPtr<UWeaponItem> InOwnerWeapon)
{
    OwnerWeapon = InOwnerWeapon;
}

FAttackData UWeaponSkillAction::CreateAttackDataBase()
{
    FAttackData AttackData;
    AttackData.Damage = GetValue(TAG_Attribute_Damage);
    AttackData.CriticalChance = GetValue(TAG_Attribute_CriticalChance);
    AttackData.CriticalMultiplier = GetValue(TAG_Attribute_CriticalMultiplier);
    AttackData.Knockback = GetValue(TAG_Attribute_Knockback);
    AttackData.Causer = Instigator;
    return AttackData;
}