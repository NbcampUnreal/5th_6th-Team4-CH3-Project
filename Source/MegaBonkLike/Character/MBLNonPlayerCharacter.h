#pragma once

#include "CoreMinimal.h"
#include "Character/MBLCharacterBase.h"
#include "Gimmick/MBLMoneyObject.h"
#include "MBLNonPlayerCharacter.generated.h"

UCLASS()
class MEGABONKLIKE_API AMBLNonPlayerCharacter : public AMBLCharacterBase
{
	GENERATED_BODY()

public:
	AMBLNonPlayerCharacter();

	UFUNCTION(BluePrintCallable)
	void HandleNavLinkJump(const FVector& Destination);

protected:
	virtual void BeginPlay() override;

	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser
	) override;

#pragma region NPC Stat 

public:
	void SetMovementSpeed(float NewSpeed);

	UPROPERTY(EditAnywhere, Category = "Stats")
	float WalkSpeed = 300.f;
	UPROPERTY(EditAnywhere, Category = "Stats")
	float RunSpeed = 600.f;

	UPROPERTY(EditDefaultsOnly, Category = "Drop")
	TSubclassOf<class AMBLMoneyObject> GoldCoin;
	//UPROPERTY(EditDefaultsOnly, Category = "Drop")
	//TSubclassOf<class AMBLExpObject> ExpCoin;

private:
	UPROPERTY(VisibleAnywhere, Category ="Stats")
	int32 Health;
	UPROPERTY(VisibleAnywhere, Category = "Stats")
	int32 MaxHealth;
	UPROPERTY(VisibleAnywhere, Category = "Stats")
	int32 Defense;
	UPROPERTY(VisibleAnywhere, Category = "Stats")
	int32 Attack;


	int32 GetHealth() const;
	int32 GetMaxHealth() const;
	int32 GetDefense() const;
	int32 GetAttack() const;
#pragma endregion 


private:
	bool bIsDead;
	void OnDeath();

	//테스트용 코드
	void KillSelf();
};
