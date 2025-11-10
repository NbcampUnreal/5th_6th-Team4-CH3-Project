#pragma once

#include "CoreMinimal.h"
#include "Character/EnemyBase.h"
#include "Gimmick/Objects/SpawnObjects/MBLMoneyObject.h"
#include "Gimmick/Objects/SpawnObjects/MBLExpObject.h"
#include "MBLEliteMonster.generated.h"

class UAnimMontage;

UCLASS()
class MEGABONKLIKE_API AMBLEliteMonster : public AEnemyBase
{
	GENERATED_BODY()

public:
	AMBLEliteMonster();

	void DeadHandle() override;

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
	UFUNCTION()
	void HandleOnCheckAttack();

	void SpawnWeapon();
	void DamageTick();
protected:
	virtual void BeginPlay() override;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<AActor> WeaponClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> AttackMontage;

	UPROPERTY()
	AActor* EquippedWeapon;

public:

	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* DamageCollider;

	UPROPERTY(EditDefaultsOnly, Category = "Drop")
	TSubclassOf<class AMBLMoneyObject> GoldCoin;
	UPROPERTY(EditDefaultsOnly, Category = "Drop")
	TSubclassOf<class AMBLExpObject> ExpCoin;

	TObjectPtr<AActor> DamageTarget;

#pragma region skill

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	float AttackRange = 1000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	float AimDuration = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	float AttackCooldown = 10.f;

	bool bIsAiming = false;
	bool bIsAttacking = false;
	bool bCanAttack = true;

	FTimerHandle AimTimer;
	FTimerHandle CooldownHandle;

	UFUNCTION()
	void CheckAttackCondition();
	UFUNCTION()
	void StartAttack();
	UFUNCTION()
	void EndAttack();
	UFUNCTION()
	void ResetAttack();
	UFUNCTION()
	void StartAiming();

#pragma endregion

private:
	bool bIsDead;
	FTimerHandle DamageTimerHandle;
	FTimerHandle WeaponEquip;
	
};
