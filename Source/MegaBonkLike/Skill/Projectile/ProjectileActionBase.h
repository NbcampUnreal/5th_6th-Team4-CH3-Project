#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Skill/Projectile/ProjectileActionContext.h"
#include "ProjectileActionBase.generated.h"

class AProjectile;

UCLASS(Abstract, Blueprintable, EditInlineNew, DefaultToInstanced)
class MEGABONKLIKE_API UProjectileActionBase : public UObject
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(TWeakObjectPtr<AProjectile> InOwnerProjectile, const FProjectileActionContext& InContext);
	// 현재 적은 Overlap, WorldStatic은 Hit (플레이어 몬스터 따로 구현)
	virtual void OnCollision(AActor* OtherActor, const FHitResult& HitResult, bool bIsOverlap) PURE_VIRTUAL(OnCollision, );		

protected:
	UPROPERTY()
	TWeakObjectPtr<AProjectile> OwnerProjectile;
	UPROPERTY()
	FProjectileActionContext Context;
};
