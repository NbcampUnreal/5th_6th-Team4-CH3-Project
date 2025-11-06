#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Attack/AttackData.h"
#include "Common/PoolSpawnable.h"
#include "DamageAreaActor.generated.h"

UCLASS(Abstract)
class MEGABONKLIKE_API ADamageAreaActor : public AActor, public IPoolSpawnable
{
	GENERATED_BODY()
	
public:
	ADamageAreaActor();

	virtual void BeginPlay() override;

	virtual void Activate() override;
	virtual void Deactivate() override;

	void SetAttackData(const FAttackData& InAttackData);
	void SetSize(float InSize);
	void SetLifeTime(float InLifeTime);
	void SetHitTimer(float Interval);

	void CheckHitOnNextFrame();
	void ApplyDamage(AActor* TargetActor);

private:
	void Shrink();
	void SetOverlapEnable(bool bInEnable);
	void CheckHit();

	void ReturnToPool();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class USphereComponent> CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FAttackData AttackData;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Size = 1.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float DamageInterval = 1.0f;

	FTimerHandle HitTimerHandle;
	FTimerHandle LifeTimeHandle;
	float LifeTime;
	float LifeTimeTimer;
};
