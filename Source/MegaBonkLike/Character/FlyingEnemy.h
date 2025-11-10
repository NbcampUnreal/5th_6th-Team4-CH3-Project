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
	UFUNCTION(Blueprintcallable)
	void SetFlyingMode(bool bNewFlying);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:
	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* DamageCollider;

	UPROPERTY(EditDefaultsOnly, Category = "Drop")
	TSubclassOf<class AMBLMoneyObject> GoldCoin;
	UPROPERTY(EditDefaultsOnly, Category = "Drop")
	TSubclassOf<class AMBLExpObject> ExpCoin;

	TWeakObjectPtr<AActor> Target;

	TObjectPtr<AActor> DamageTarget;

	UPROPERTY(VisibleAnywhere, Category = "State")
	bool bIsFlyingMode = true;

private:
	bool bIsDead;
	FTimerHandle DamageTimerHandle;
	FTimerHandle MoveTimerHandle;
	FTimerHandle TrackTimerHandle;

	FVector CurrentDirection;

	/*테스트용 코드
	void KillSelf();*/
};
