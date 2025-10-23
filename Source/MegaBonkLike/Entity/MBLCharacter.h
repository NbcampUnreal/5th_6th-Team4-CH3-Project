#pragma once

#include "CoreMinimal.h"
#include "Entity/MBLEntity.h"
#include "MBLCharacter.generated.h"

class UMBLPlayerInputConfig;
class USpringArmComponent;
class UCameraComponent;
class UInventoryComponent;
class USkillComponent;
class UAttributeComponent;

struct FInputActionValue;

UCLASS()
class MEGABONKLIKE_API AMBLCharacter : public AMBLEntity
{
	GENERATED_BODY()
	
public:
	AMBLCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	float GetAttributeValue(const FGameplayTag& AttributeTag) const;

protected:
	void Input_Move(const FInputActionValue& InputValue);
	void Input_Look(const FInputActionValue& InputValue);
	void InputTempAcquireItem();

	void RecalculateSpeed();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USpringArmComponent> SpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCameraComponent> Camera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UAttributeComponent> AttributeComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UInventoryComponent> Inventory;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USkillComponent> SkillComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMBLPlayerInputConfig> InputConfig;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float NormalSpeed;
};
