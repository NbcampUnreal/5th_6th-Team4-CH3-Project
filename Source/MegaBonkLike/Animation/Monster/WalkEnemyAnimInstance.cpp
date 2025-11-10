#include "Animation/Monster/WalkEnemyAnimInstance.h"
#include "Character/MBLCharacterBase.h"
#include "Character/MBLBossCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Character/MBLNonPlayerCharacter.h"

void UWalkEnemyAnimInstance::NativeInitializeAnimation()
{
	APawn* OwnerPawn = TryGetPawnOwner();
	if (IsValid(OwnerPawn) == true)
	{
		OwnerCharacter = Cast<AMBLCharacterBase>(OwnerPawn);
		OwnerCharacterMovement = OwnerCharacter->GetCharacterMovement();
	}
}

void UWalkEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (IsValid(OwnerCharacter) == true && IsValid(OwnerCharacterMovement) == true)
	{
		Velocity = OwnerCharacterMovement->Velocity;
		GroundSpeed = UKismetMathLibrary::VSizeXY(Velocity);

		float GroundAcceleration = UKismetMathLibrary::VSizeXY(OwnerCharacterMovement->GetCurrentAcceleration());
		bool bIsAccelerationNearlyZero = FMath::IsNearlyZero(GroundAcceleration);
		bShouldMove = (KINDA_SMALL_NUMBER < GroundSpeed) && (bIsAccelerationNearlyZero == false);
		
		if (AMBLNonPlayerCharacter* OwnerNPC = Cast<AMBLNonPlayerCharacter>(OwnerCharacter))
		{
			bShouldMove = KINDA_SMALL_NUMBER < GroundSpeed;
		}

		if (AMBLBossCharacter* OwnerNPC = Cast<AMBLBossCharacter>(OwnerCharacter))
		{
			bShouldMove = KINDA_SMALL_NUMBER < GroundSpeed;
		}
	}
}

