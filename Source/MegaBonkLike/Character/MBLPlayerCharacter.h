#pragma once

#include "CoreMinimal.h"
#include "Character/MBLCharacterBase.h"
#include "InputActionValue.h"
#include "MBLPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UMBLInputConfig;
class UInputMappingContext;

UCLASS()
class MEGABONKLIKE_API AMBLPlayerCharacter : public AMBLCharacterBase
{
	GENERATED_BODY()
	
#pragma region Override ACharacter

public:
	AMBLPlayerCharacter();

	virtual void BeginPlay() override;

protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<UCameraComponent> CameraComponent;

#pragma endregion

#pragma region Input

private:
	void InputMove(const FInputActionValue& InValue);
	void InputLook(const FInputActionValue& InValue);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<UMBLInputConfig> PlayerCharacterInputConfig;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<UInputMappingContext> PlayerCharacterInputMappingContext;

#pragma endregion	
};
