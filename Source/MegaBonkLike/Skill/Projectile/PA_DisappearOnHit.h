#pragma once

#include "CoreMinimal.h"
#include "Skill/Projectile/ProjectileActionBase.h"
#include "PA_DisappearOnHit.generated.h"

UCLASS()
class MEGABONKLIKE_API UPA_DisappearOnHit : public UProjectileActionBase
{
	GENERATED_BODY()
	
public:
	virtual void OnCollision(AActor* OtherActor, const FHitResult& HitResult, bool bIsOverlap) override;
};
