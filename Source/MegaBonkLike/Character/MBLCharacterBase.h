#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MBLCharacterBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnChangedProgressValue, float, CurrValue, float, MaxValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDead);

UCLASS()
class MEGABONKLIKE_API AMBLCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AMBLCharacterBase();

protected:
	virtual void BeginPlay() override;
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void SetMaxHP(float InMaxHP);
	void AddHealth(float Heal);
	void UpdateCurrHP(float InCurrHP);

private:
	void SetCameraCollisionIgnore();

public:
	FOnChangedProgressValue OnHPChanged;
	FOnDead OnDead;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float MaxHP = 500.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float CurrHP = 500.0f;
};
