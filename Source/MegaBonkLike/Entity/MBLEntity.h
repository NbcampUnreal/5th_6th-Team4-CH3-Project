#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include "MBLEntity.generated.h"

class UAttributeComponent;

UCLASS()
class MEGABONKLIKE_API AMBLEntity : public ACharacter
{
	GENERATED_BODY()

public:
	AMBLEntity();

protected:
	virtual void BeginPlay() override;
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:
	float GetAttributeValue(const FGameplayTag& AttributeTag) const;

private:
	void SetCameraCollisionIgnore();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UAttributeComponent> AttributeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float CurrHP = 500.0f;
};
