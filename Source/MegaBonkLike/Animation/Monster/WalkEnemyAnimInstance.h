#pragma once
#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "WalkEnemyAnimInstance.generated.h"

class AMBLCharacterBase;
class UCharacterMovementComponent;

UCLASS()
class MEGABONKLIKE_API UWalkEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<AMBLCharacterBase> OwnerCharacter;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UCharacterMovementComponent> OwnerCharacterMovement;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FVector Velocity;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float GroundSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	uint8 bShouldMove : 1;

};
