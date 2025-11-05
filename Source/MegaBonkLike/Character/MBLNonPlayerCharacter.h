#pragma once

#include "CoreMinimal.h"
#include "Character/EnemyBase.h"
#include "Gimmick/Objects/SpawnObjects/MBLMoneyObject.h"
#include "Gimmick/Objects/SpawnObjects/MBLExpObject.h"
#include "MBLNonPlayerCharacter.generated.h"

UCLASS()
class MEGABONKLIKE_API AMBLNonPlayerCharacter : public AEnemyBase
{
	GENERATED_BODY()

public:
	AMBLNonPlayerCharacter();

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
	
	void DamageTick();

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

	/*테스트용 코드
	void KillSelf();*/
};
