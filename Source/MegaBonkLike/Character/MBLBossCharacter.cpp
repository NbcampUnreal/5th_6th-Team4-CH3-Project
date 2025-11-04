#include "Character/MBLBossCharacter.h"
#include "AI/MBLAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "MonsterAttack/GroundAttack.h"
#include "Kismet/GameplayStatics.h"
#include "Game/MBLGameMode.h"
#include "MegaBonkLike.h"

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

	//감지용 캡슐컴포
	DamageCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("DamageCollider"));
	DamageCollider->SetupAttachment(GetMesh());
	DamageCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DamageCollider->SetCollisionObjectType(ECC_WorldDynamic);
	DamageCollider->SetCollisionResponseToAllChannels(ECR_Ignore);
	DamageCollider->SetCollisionResponseToChannel(ECC_MBL_PLAYER, ECR_Overlap);
	DamageCollider->SetRelativeLocation(FVector(0.f, 0.f, 90.f));
	DamageCollider->SetCapsuleRadius(45.f);
	DamageCollider->SetCapsuleHalfHeight(100.f);

	DamageCollider->OnComponentBeginOverlap.AddDynamic(this, &AMBLBossCharacter::OnDamageColliderBeginOverlap);
	DamageCollider->OnComponentEndOverlap.AddDynamic(this, &AMBLBossCharacter::OnDamageColliderEndOverlap);

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
	Attack = 20;

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


void AMBLBossCharacter::DeadHandle()
{
	Super::DeadHandle();

	//if (AMBLGameMode* GameMode = Cast<AMBLGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
	//{
	//	//GameMode->DeadBoss(); // 보스 사망시
	//}

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

void AMBLBossCharacter::OnDamageColliderBeginOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 TherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (!OtherActor || !OtherComp) return;

	if (OtherComp->GetCollisionObjectType() == ECC_MBL_PLAYER)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Player Detected."));
		DamageTarget = OtherActor;

		if (!GetWorldTimerManager().IsTimerActive(DamageTimerHandle))
		{
			GetWorldTimerManager().SetTimer(
				DamageTimerHandle,
				this,
				&AMBLBossCharacter::DamageTick,
				0.5f,
				true,
				0.f
			);
		}
	}
}

void AMBLBossCharacter::OnDamageColliderEndOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor == DamageTarget)
	{
		DamageTarget = nullptr;
		GetWorldTimerManager().ClearTimer(DamageTimerHandle);
		//UE_LOG(LogTemp, Warning, TEXT("Player left dectection area."));
	}
}

void AMBLBossCharacter::DamageTick()
{
	if (DamageTarget)
	{
		UGameplayStatics::ApplyDamage(DamageTarget, Attack, GetInstigatorController(), GetInstigator(), UDamageType::StaticClass());
	}
	else
	{
		GetWorldTimerManager().ClearTimer(DamageTimerHandle);
	}
}