#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageAreaActor.generated.h"

UCLASS(Abstract)
class MEGABONKLIKE_API ADamageAreaActor : public AActor
{
	GENERATED_BODY()
	
public:
	ADamageAreaActor();

	virtual void BeginPlay() override;

	void SetDamage(float InDamage);
	void SetSize(float InSize);
	void SetLifeTime(float InLifeTime);

	void CheckHitOnNextFrame();
	void ApplyDamage(AActor* TargetActor);

private:
	void Shrink();
	void SetOverlapEnable(bool bInEnable);
	void CheckHit();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class USphereComponent> CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Damage;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Size = 1.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float DamageInterval = 1.0f;

	FTimerHandle HitTimerHandle;
	FTimerHandle LifeTimeHandle;
	float LifeTime;
	float LifeTimeTimer;
};
