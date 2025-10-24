#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "KIH_NpcAnimInstance.generated.h"

class AMBLCharacterBase;
class UCharacterMovementComponent;

UCLASS()
class MEGABONKLIKE_API UKIH_NpcAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<AMBLCharacterBase> NPC1;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UCharacterMovementComponent> NPC1Movement;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FVector Velocity;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float GroundSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	uint8 bShouldMove : 1;
	
};
