#pragma once

#include "CoreMinimal.h"
#include "Character/MBLCharacterBase.h"
#include "MBLBossCharacter.generated.h"

UCLASS()
class MEGABONKLIKE_API AMBLBossCharacter : public AMBLCharacterBase
{
	GENERATED_BODY()

public:
	AMBLBossCharacter();

	void GroundAttack();

protected:
	virtual void BeginPlay() override;


#pragma region NPC Stat 

public:
	void SetMovementSpeed(float NewSpeed);

	UPROPERTY(EditAnywhere, Category = "Stats")
	float WalkSpeed = 300.f;
	UPROPERTY(EditAnywhere, Category = "Stats")
	float RunSpeed = 600.f;

private:
	UPROPERTY(VisibleAnywhere, Category = "Stats")
	int32 Attack;
#pragma endregion 

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

	FTimerHandle AttackTimerHandle;
};