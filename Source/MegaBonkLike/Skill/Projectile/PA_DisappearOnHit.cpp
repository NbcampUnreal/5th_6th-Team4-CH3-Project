#include "Skill/Projectile/PA_DisappearOnHit.h"
#include "Skill/Projectile.h"

void UPA_DisappearOnHit::OnCollision(AActor* OtherActor, const FHitResult& HitResult, bool bIsOverlap)
{
	if (OwnerProjectile.IsValid() == false)
		return;

	OwnerProjectile->ReturnToPool();
}
