#include "Skill/Skill.h"
#include "Skill/SkillActionBase.h"
#include "Item/WeaponItem.h"
#include "Attribute/AttributeModifier.h"

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
		SkillAction->SetOwnerWeapon(OwnerWeapon);
	}
}
