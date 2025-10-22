#include "Animation/MBLAnimInstance.h"
#include "Entity/MBLCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UMBLAnimInstance::NativeInitializeAnimation()
{
	APawn* OwnerPawn = TryGetPawnOwner();
	if (IsValid(OwnerPawn) == true)
	{
		OwnerCharacter = Cast<AMBLCharacter>(OwnerPawn);
		OwnerCharacterMovement = OwnerCharacter->GetCharacterMovement();
	}
}

void UMBLAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (IsValid(OwnerCharacter) == true && IsValid(OwnerCharacterMovement) == true)
	{
		Velocity = OwnerCharacterMovement->Velocity;
		GroundSpeed = Velocity.Size2D();

		float GroundAcceleration = OwnerCharacterMovement->GetCurrentAcceleration().Size2D();
		bool bIsAccelerationNearlyZero = FMath::IsNearlyZero(GroundAcceleration);
		bShouldMove = (KINDA_SMALL_NUMBER < GroundSpeed) && (bIsAccelerationNearlyZero == false);
		bIsFalling = OwnerCharacterMovement->IsFalling();
	}
}
