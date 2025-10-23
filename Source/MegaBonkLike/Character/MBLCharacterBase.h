#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MBLCharacterBase.generated.h"

UCLASS()
class MEGABONKLIKE_API AMBLCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AMBLCharacterBase();

protected:
	virtual void BeginPlay() override;
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

private:
	void SetCameraCollisionIgnore();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float CurrHP = 500.0f;
};
