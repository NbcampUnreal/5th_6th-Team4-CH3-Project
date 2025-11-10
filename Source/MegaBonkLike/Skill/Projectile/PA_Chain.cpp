#include "Skill/Projectile/PA_Chain.h"
#include "Skill/Projectile.h"
#include "MegaBonkLike.h"
#include "Engine/OverlapResult.h"

void UPA_Chain::Initialize(TWeakObjectPtr<AProjectile> InOwnerProjectile, const FProjectileActionContext& InContext)
{
	Super::Initialize(InOwnerProjectile, InContext);

	ChainCount = 1;
	AttackedTargets.Empty();
}

void UPA_Chain::OnCollision(AActor* OtherActor, const FHitResult& HitResult, bool bIsOverlap)
{
	if (OwnerProjectile.IsValid() == false)
		return;

	if (bIsOverlap == false)
	{
		OwnerProjectile->ReturnToPool();
		return;
	}

	AttackedTargets.Add(OtherActor);

	// 첫 타도 ChainCount에 포함
	if (ChainCount >= Context.ChainCount)
	{
		OwnerProjectile->ReturnToPool();
		return;
	}

	++ChainCount;
	AActor* NewTarget = FindNearestTarget(OtherActor->GetActorLocation());
	if (IsValid(NewTarget) == false)
	{
		OwnerProjectile->ReturnToPool();
		return;
	}

	FVector Dir = (NewTarget->GetActorLocation() - OwnerProjectile->GetActorLocation()).GetSafeNormal();
	OwnerProjectile->SetDirectionAndSpeed(Dir, OwnerProjectile->GetVelocity().Size());
}

AActor* UPA_Chain::FindNearestTarget(const FVector& FromLocation)
{
	FVector Origin = OwnerProjectile->GetActorLocation();
	TArray<FOverlapResult> Overlaps;
	FCollisionShape CollisionShape;
	CollisionShape.SetSphere(ChainRadius);
	bool bHit = OwnerProjectile->GetWorld()->OverlapMultiByObjectType(
		Overlaps,
		Origin,
		FQuat::Identity,
		ECC_MBL_ENEMY,
		CollisionShape);

	AActor* TargetActor = nullptr;
	if (bHit)
	{
		for (auto& Result : Overlaps)
		{
			AActor* HitActor = Result.GetActor();
			if (IsValid(HitActor) == false || HitActor == OwnerProjectile)
				continue;

			if (AttackedTargets.Contains(HitActor) == true)
				continue;

			TargetActor = HitActor;
			break;
		}
	}

	return TargetActor;
}

