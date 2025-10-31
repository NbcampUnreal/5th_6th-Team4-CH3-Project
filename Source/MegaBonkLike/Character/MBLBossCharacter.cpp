#include "Character/MBLBossCharacter.h"
#include "AI/MBLAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "MonsterAttack/GroundAttack.h"
#include "Kismet/GameplayStatics.h"

AMBLBossCharacter::AMBLBossCharacter()
{
	AIControllerClass = AMBLAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	float CharacterHalfHeight = 180.f;
	float CharacterRadius = 80.f;

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
	MaxHP = 1000;
	CurrHP = MaxHP;
	Attack = 50;

}


void AMBLBossCharacter::BeginPlay()
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
	//공격로직테스트용
	//GroundAttack();
}

void AMBLBossCharacter::SetMovementSpeed(float NewSpeed)
{
	if (UCharacterMovementComponent* Movement = GetCharacterMovement())
	{
		Movement->MaxWalkSpeed = NewSpeed;
		UE_LOG(LogTemp, Warning, TEXT("Speed changed: %.1f"), NewSpeed);
	}
}

void AMBLBossCharacter::DeadHandle()
{
	if (bIsDead) return;

	bIsDead = true;

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->DisableMovement();

	Destroy();
	UE_LOG(LogTemp, Warning, TEXT("Died."));
}

void AMBLBossCharacter::GroundAttack()
{
	if (!GroundAttackClass) return;

	CurrentAttackCount = 0;

	SpawnGroundAttack();

	GetWorld()->GetTimerManager().SetTimer(
		AttackTimerHandle,
		this,
		&AMBLBossCharacter::SpawnGroundAttack,
		AttackInterval,
		true
	);
}

void AMBLBossCharacter::SpawnGroundAttack()
{
	if (CurrentAttackCount >= AttackRepeatCount)
	{
		GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);
		return;
	}

	CurrentAttackCount++;

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!PlayerPawn) return;

	FVector SpawnLocation = PlayerPawn->GetActorLocation();
	FRotator SpawnRotation = FRotator::ZeroRotator;

	FActorSpawnParameters Params;
	Params.Owner = this;


	AGroundAttack* SkillArea = GetWorld()->SpawnActor<AGroundAttack>(
		GroundAttackClass,
		SpawnLocation,
		SpawnRotation,
		Params
	);

	if (SkillArea)
	{
		SkillArea->Initialize(this);
	}
}
