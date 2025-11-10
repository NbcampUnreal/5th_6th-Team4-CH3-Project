#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EliteEnemyAnimInstance.generated.h"

class AMBLCharacterBase;
class UCharacterMovementComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCheckAttack);

UCLASS()
class MEGABONKLIKE_API UEliteEnemyAnimInstance : public UAnimInstance
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

private:
	void AnimNotify_CheckHit();

public:

	FOnCheckAttack OnCheckAttack;
};
