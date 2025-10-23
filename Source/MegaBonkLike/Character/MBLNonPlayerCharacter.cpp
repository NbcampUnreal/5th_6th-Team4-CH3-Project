#include "Character/MBLNonPlayerCharacter.h"
#include "Controller/MBLAIController.h"
#include "GameFramework/CharacterMovementComponent.h"

AMBLNonPlayerCharacter::AMBLNonPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = AMBLAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AMBLNonPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (false == IsPlayerControlled())
	{
		bUseControllerRotationYaw = false;

		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->RotationRate = FRotator(0.f, 480.f, 0.f);

		GetCharacterMovement()->MaxWalkSpeed = 300.f;
	}
}
