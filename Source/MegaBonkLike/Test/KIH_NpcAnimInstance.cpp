#include "Test/KIH_NpcAnimInstance.h"
#include "Character/MBLCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UKIH_NpcAnimInstance::NativeInitializeAnimation()
{
	APawn* OwnerPawn = TryGetPawnOwner();
	if (IsValid(OwnerPawn) == true)
	{
		NPC1 = Cast<AMBLCharacterBase>(OwnerPawn);
		NPC1Movement = NPC1->GetCharacterMovement();
	}
}

void UKIH_NpcAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (IsValid(NPC1) == true && IsValid(NPC1Movement) == true)
	{
		Velocity = NPC1Movement->Velocity;
		GroundSpeed = UKismetMathLibrary::VSizeXY(Velocity);

		float GroundAcceleration = UKismetMathLibrary::VSizeXY(NPC1Movement->GetCurrentAcceleration());
		bool bIsAccelerationNearlyZero = FMath::IsNearlyZero(GroundAcceleration);
		bShouldMove = (KINDA_SMALL_NUMBER < GroundSpeed) && (bIsAccelerationNearlyZero == false);
	}
}

