#include "Skill/Projectile/ProjectileActionBase.h"

void UProjectileActionBase::Initialize(TWeakObjectPtr<AProjectile> InOwnerProjectile, const FProjectileActionContext& InContext)
{
	OwnerProjectile = InOwnerProjectile;
	Context = InContext;
}
