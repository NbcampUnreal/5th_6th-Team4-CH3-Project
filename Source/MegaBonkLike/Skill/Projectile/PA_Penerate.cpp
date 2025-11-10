#include "Skill/Projectile/PA_Penerate.h"
#include "Skill/Projectile.h"

void UPA_Penerate::OnCollision(AActor* OtherActor, const FHitResult& HitResult, bool bIsOverlap)
{
	if (OwnerProjectile.IsValid() == false)
		return;

	if (bIsOverlap == false)
	{
		OwnerProjectile->ReturnToPool();
	}
}
