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
	void DetectEnemy();
	void ShootRandomTarget();
	void Shoot(const FVector& TargetPos);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AProjectile> ProjectileClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BaseDamage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BaseProjectileSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BaseProjectileCount;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName TargetTag;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AutoDetectRadius;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bAutoDetect;

	FTimerHandle AttackIntervalHandle;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 AttackCount = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float AttackInterval;

	TArray<TWeakObjectPtr<AActor>> AttackTargets;
};
