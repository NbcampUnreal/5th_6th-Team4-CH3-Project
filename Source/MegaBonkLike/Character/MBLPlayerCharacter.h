#pragma once

#include "CoreMinimal.h"
#include "Character/MBLCharacterBase.h"
#include "GameplayTagContainer.h"
#include "MBLPlayerCharacter.generated.h"

class UMBLPlayerInputConfig;
class USpringArmComponent;
class UCameraComponent;
class UInventoryComponent;
class USkillComponent;
class UAttributeComponent;

struct FInputActionValue;

UCLASS()
class MEGABONKLIKE_API AMBLPlayerCharacter : public AMBLCharacterBase
{
	GENERATED_BODY()
	
public:
	AMBLPlayerCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	void AddAttributeChangedCallback(const FGameplayTag& Tag, TWeakObjectPtr<UObject> InInstigator, TFunction<void(const TWeakObjectPtr<class UAttribute>)> NewCallBack);
	void RemoveAttributeChangedCallback(const FGameplayTag& Tag, TWeakObjectPtr<UObject> InInstigator);
	float GetAttributeValue(const FGameplayTag& AttributeTag) const;

	FVector GetFootLocation() const;

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
