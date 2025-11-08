#pragma once

#include "CoreMinimal.h"
#include "Skill/WeaponSkillAction.h"
#include "WSA_RangeAttack.generated.h"

class AProjectile;
class UProjectileActionBase;

UCLASS(BlueprintType)
class MEGABONKLIKE_API UWSA_RangeAttack : public UWeaponSkillAction
{
	GENERATED_BODY()

public:
	virtual void Activate(TWeakObjectPtr<AActor> InInstigator) override;

private:
	void StartShoot();
	void ExecuteShoot();

	void DetectEnemy();
	void ShootRandomTarget();

	void Shoot(const FVector& TargetDir);
	// 분산
	void ShootSpread(const FVector& TargetDir);
	// 단일
	void ShootSingle(const FVector& TargetDir);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AProjectile> ProjectileClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UProjectileActionBase> ProjectileActionClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BaseLifeTime = 3.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 BaseChainCount = 4;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AutoDetectRadius;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bAutoDetect;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bShootSpread;

	FTimerHandle AttackIntervalHandle;
	UPROPERTY(BlueprintReadOnly)
	int32 ProjectileCount = 0;
	UPROPERTY(BlueprintReadOnly)
	float AttackInterval;

	TArray<TWeakObjectPtr<AActor>> AttackTargets;
};
