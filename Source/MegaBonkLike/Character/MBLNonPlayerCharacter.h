#pragma once

#include "CoreMinimal.h"
#include "Character/MBLCharacterBase.h"
#include "Gimmick/Objects/SpawnObjects/MBLMoneyObject.h"
#include "Gimmick/Objects/SpawnObjects/MBLExpObject.h"
#include "MBLNonPlayerCharacter.generated.h"

UCLASS()
class MEGABONKLIKE_API AMBLNonPlayerCharacter : public AMBLCharacterBase
{
	GENERATED_BODY()

public:
	AMBLNonPlayerCharacter();

protected:
	virtual void BeginPlay() override;

#pragma region NPC Stat 

public:
	void SetMovementSpeed(float NewSpeed);

	UPROPERTY(EditAnywhere, Category = "Stats")
	float WalkSpeed = 300.f;
	UPROPERTY(EditAnywhere, Category = "Stats")
	float RunSpeed = 600.f;

	UPROPERTY(EditDefaultsOnly, Category = "Drop")
	TSubclassOf<class AMBLMoneyObject> GoldCoin;
	UPROPERTY(EditDefaultsOnly, Category = "Drop")
	TSubclassOf<class AMBLExpObject> ExpCoin;

private:
	UPROPERTY(VisibleAnywhere, Category = "Stats")
	int32 Attack;

#pragma endregion 


private:
	bool bIsDead;
	UFUNCTION()
	void OnDeath();

	//테스트용 코드
	void KillSelf();
};
