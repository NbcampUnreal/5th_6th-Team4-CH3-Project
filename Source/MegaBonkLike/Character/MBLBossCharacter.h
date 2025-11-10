#pragma once

#include "CoreMinimal.h"
#include "Character/EnemyBase.h"
#include "MBLBossCharacter.generated.h"

class UCapsuleComponent;

UCLASS()
class MEGABONKLIKE_API AMBLBossCharacter : public AEnemyBase
{
	GENERATED_BODY()

public:
	AMBLBossCharacter();

	void GroundAttack();

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

public:
	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* DamageCollider;

	TObjectPtr<AActor> DamageTarget;

protected:
	virtual void BeginPlay() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

private:
	UPROPERTY(EditDefaultsOnly, Category="Skill")
	TSubclassOf<class AGroundAttack> GroundAttackClass;
	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	float AttackInterval = 0.8f;
	UPROPERTY(EditDefaultsOnly, Category = "Skill")
	int32 AttackRepeatCount = 5;

	int32 CurrentAttackCount = 0;

private:
	bool bIsDead;
	void DeadHandle() override;
	void SpawnGroundAttack();

	FTimerHandle AttackTimerHandle;   //스킬공격
	FTimerHandle DamageTimerHandle;   //근접피격
};