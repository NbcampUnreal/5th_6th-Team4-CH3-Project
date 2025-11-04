#pragma once

#include "CoreMinimal.h"
#include "Skill/WeaponSkillAction.h"
#include "WSA_RangeAttack.generated.h"

class AProjectile;

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
	float AutoDetectRadius;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bAutoDetect;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bShootSpread;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bPenetrate;

	FTimerHandle AttackIntervalHandle;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 ProjectileCount = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float AttackInterval;

	TArray<TWeakObjectPtr<AActor>> AttackTargets;
};
