#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MBLBossCharacter.generated.h"

UCLASS()
class MEGABONKLIKE_API AMBLBossCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMBLBossCharacter();

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

private:
	UPROPERTY(VisibleAnywhere, Category = "Stats")
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

};
