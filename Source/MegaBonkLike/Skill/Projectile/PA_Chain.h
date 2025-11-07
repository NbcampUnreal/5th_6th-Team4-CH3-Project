#pragma once

#include "CoreMinimal.h"
#include "Skill/Projectile/ProjectileActionBase.h"
#include "PA_Chain.generated.h"

UCLASS()
class MEGABONKLIKE_API UPA_Chain : public UProjectileActionBase
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(TWeakObjectPtr<AProjectile> InOwnerProjectile, const FProjectileActionContext& InContext) override;
	virtual void OnCollision(AActor* OtherActor, const FHitResult& HitResult, bool bIsOverlap) override;

	AActor* FindNearestTarget(const FVector& FromLocation);

protected:
	UPROPERTY(EditAnywhere)
	int32 ChainRadius = 1000.0f;
	UPROPERTY()
	int32 ChainCount = 1;

	UPROPERTY()
	TArray<TWeakObjectPtr<AActor>> AttackedTargets;
};
