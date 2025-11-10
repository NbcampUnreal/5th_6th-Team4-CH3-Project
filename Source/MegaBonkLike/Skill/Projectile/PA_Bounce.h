#pragma once

#include "CoreMinimal.h"
#include "Skill/Projectile/ProjectileActionBase.h"
#include "PA_Bounce.generated.h"

UCLASS()
class MEGABONKLIKE_API UPA_Bounce : public UProjectileActionBase
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(TWeakObjectPtr<AProjectile> InOwnerProjectile, const FProjectileActionContext& InContext) override;
	virtual void OnCollision(AActor* OtherActor, const FHitResult& HitResult, bool bIsOverlap) override;

	UFUNCTION()
	void ReturnToInstigator();

protected:
	UPROPERTY()
	bool bReturned;
	UPROPERTY()
	FTimerHandle BounceTimerHandle;
};
