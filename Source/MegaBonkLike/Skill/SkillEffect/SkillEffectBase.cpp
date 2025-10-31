#include "Skill/SkillEffect/SkillEffectBase.h"

void USkillEffectBase::SetOwner(TWeakObjectPtr<AActor> InOwner)
{
	Owner = InOwner;
}
