#pragma once

#include "CoreMinimal.h"
#include "Character/EnemyBase.h"
#include "Gimmick/Objects/SpawnObjects/MBLMoneyObject.h"
#include "Gimmick/Objects/SpawnObjects/MBLExpObject.h"
#include "Character/MonsterStat.h"
#include "FlyingEnemy.generated.h"

UCLASS()
class MEGABONKLIKE_API AFlyingEnemy : public AEnemyBase
{
	GENERATED_BODY()

public:
	AFlyingEnemy();

	void DeadHandle() override;

	void SetSpeed(EMBLWaveState Wave) override;

	UFUNCTION()
	void OnDamageColliderBeginOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 TherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);
	UFUNCTION()
	void OnDamageColliderEndOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

	void DamageTick();

	void UpdateTrack();
	void MoveStep();

protected:
	virtual void BeginPlay() override;


public:

	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* DamageCollider;

	UPROPERTY(EditDefaultsOnly, Category = "Drop")
	TSubclassOf<class AMBLMoneyObject> GoldCoin;
	UPROPERTY(EditDefaultsOnly, Category = "Drop")
	TSubclassOf<class AMBLExpObject> ExpCoin;

	TObjectPtr<AActor> DamageTarget;

private:
	bool bIsDead;
	FTimerHandle DamageTimerHandle;

	FVector CurrentDirection;
	TWeakObjectPtr<AActor> Target;

	/*테스트용 코드
	void KillSelf();*/
};
