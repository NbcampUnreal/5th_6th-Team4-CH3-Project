#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MBLAnimInstance.generated.h"

class AMBLCharacter;
class UCharacterMovementComponent;

UCLASS()
class MEGABONKLIKE_API UMBLAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<AMBLCharacter> OwnerCharacter;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCharacterMovementComponent> OwnerCharacterMovement;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector Velocity;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float GroundSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	uint8 bShouldMove : 1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	uint8 bIsFalling : 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 bIsDead : 1;
};
