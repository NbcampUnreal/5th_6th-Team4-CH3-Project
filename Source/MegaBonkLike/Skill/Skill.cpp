#include "Skill/Skill.h"
#include "Item/WeaponItem.h"
#include "Skill/WeaponSkillAction.h"

void USkill::Activate(AActor* InInstigator)
{
	Instigator = InInstigator;
	for (auto& SkillAction : SkillActions)
	{
		SkillAction->Activate(Instigator);
	}
}

void USkill::Tick(float DeltaTime)
{
	for (auto& SkillAction : SkillActions)
	{
		if (SkillAction->GetUseTick() == true)
		{
			SkillAction->Tick(DeltaTime);
		}
	}
}

void USkill::Deactivate()
{
	for (auto& SkillAction : SkillActions)
	{
		SkillAction->Deactivate();
	}
}

void USkill::SetOwnerWeapon(UWeaponItem* InOwnerWeapon)
{
	OwnerWeapon = InOwnerWeapon;
	for (auto& SkillAction : SkillActions)
	{
		if (UWeaponSkillAction* WeaponSkillAction = (UWeaponSkillAction*)SkillAction)
		{
			WeaponSkillAction->SetOwnerWeapon(OwnerWeapon);
		}
	}
}
