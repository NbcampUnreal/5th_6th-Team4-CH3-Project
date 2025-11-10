#include "Skill/SkillEffect/SkillEffectSet.h"
#include "Skill/SkillEffect/SkillEffectBase.h"

void FSkillEffectSet::SetOwner(TWeakObjectPtr<AActor> InOwner)
{
	for (const auto& Effect : Effects)
	{
		Effect->SetOwner(InOwner);
	}
}

void FSkillEffectSet::ActivateAll(const FVector& Location, const FRotator& Rotation, float InScale)
{
	for (const auto& Effect : Effects)
	{
		Effect->Activate(Location, Rotation, InScale);
	}
}

void FSkillEffectSet::DeactivateAll()
{
	for (const auto& Effect : Effects)
	{
		Effect->Deactivate();
	}
}
