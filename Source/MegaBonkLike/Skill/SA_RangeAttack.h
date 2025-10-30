#pragma once

#include "CoreMinimal.h"
#include "Skill/SkillActionBase.h"
#include "SA_RangeAttack.generated.h"

class AProjectile;

UCLASS(BlueprintType)
class MEGABONKLIKE_API USA_RangeAttack : public USkillActionBase
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
	FName TargetTag;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AutoDetectRadius;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bAutoDetect;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bShootSpread;

	FTimerHandle AttackIntervalHandle;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 ProjectileCount = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float AttackInterval;

	TArray<TWeakObjectPtr<AActor>> AttackTargets;
};
