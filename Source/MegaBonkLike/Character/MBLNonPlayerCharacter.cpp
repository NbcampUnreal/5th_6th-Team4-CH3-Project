#include "Character/MBLNonPlayerCharacter.h"
#include "AI/MBLAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

AMBLNonPlayerCharacter::AMBLNonPlayerCharacter()
{
	AIControllerClass = AMBLAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	float CharacterHalfHeight = 90.f;
	float CharacterRadius = 40.f;

	GetCapsuleComponent()->InitCapsuleSize(CharacterRadius, CharacterHalfHeight);

	FVector PivotPosition(0.f, 0.f, -CharacterHalfHeight);
	FRotator PivotRotation(0.f, -90.f, 0.f);
	GetMesh()->SetRelativeLocationAndRotation(PivotPosition, PivotRotation);

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.5f;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);

	GetCharacterMovement()->bUseRVOAvoidance = true;
	GetCharacterMovement()->AvoidanceWeight = 1.f;

	bIsDead = false;
	MaxHP = 100;
	CurrHP = MaxHP;
	Attack = 50;
}

void AMBLNonPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (false == IsPlayerControlled())
	{
		bUseControllerRotationYaw = false;

		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->RotationRate = FRotator(0.f, 480.f, 0.f);

		GetCharacterMovement()->MaxWalkSpeed = 300.f;
	}

	//KillSelf();       몬스터죽음 테스트용
}


void AMBLNonPlayerCharacter::SetMovementSpeed(float NewSpeed)
{
	if (UCharacterMovementComponent* Movement = GetCharacterMovement())
	{
		Movement->MaxWalkSpeed = NewSpeed;
		UE_LOG(LogTemp, Warning, TEXT("Speed changed: %.1f"), NewSpeed);
	}
}

void AMBLNonPlayerCharacter::DeadHandle()
{
	Super::DeadHandle();

	if (bIsDead) return;

	bIsDead = true;

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->DisableMovement();

	FVector GoldSpawnLocation = GetActorLocation() + FVector(25.f, 0.f, 0.f);
	FVector ExpSpawnLocation = GetActorLocation() + FVector(-25.f, 0.f, 0.f);
	if (GoldCoin)
	{
		GetWorld()->SpawnActor<AMBLMoneyObject>(GoldCoin, GoldSpawnLocation, FRotator::ZeroRotator);
	}
	
	if (ExpCoin)
	{
		GetWorld()->SpawnActor<AMBLExpObject>(ExpCoin, ExpSpawnLocation, FRotator::ZeroRotator);
	}

	Destroy();
	UE_LOG(LogTemp, Warning, TEXT("Died."));
}

//테스트용 코드
void AMBLNonPlayerCharacter::KillSelf()
{
	DeadHandle();
}
