#include "Skill/WeaponSkillAction.h"
#include "Item/WeaponItem.h"

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