#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include "MBLEntity.generated.h"

UCLASS()
class MEGABONKLIKE_API AMBLEntity : public ACharacter
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

private:
	void SetCameraCollisionIgnore();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float CurrHP = 500.0f;
};
