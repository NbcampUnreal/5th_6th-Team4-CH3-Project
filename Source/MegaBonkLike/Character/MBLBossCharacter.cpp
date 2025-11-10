#include "Character/MBLBossCharacter.h"
#include "AI/MBLAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "MonsterAttack/GroundAttack.h"
#include "Kismet/GameplayStatics.h"
#include "Game/MBLGameMode.h"
#include "MegaBonkLike.h"
#include "Player/MBLPlayerController.h" //IngameUI 추가
#include "Kismet/GameplayStatics.h" //IngameUI 추가

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
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
	GetCharacterMovement()->bUseRVOAvoidance = true;
	GetCharacterMovement()->AvoidanceWeight = 1.f;

	bUseControllerRotationYaw = false;

	bIsDead = false;
	MaxHP = 1000;
	CurrHP = MaxHP;
	Attack = 20;

}


void AMBLBossCharacter::BeginPlay()
{
	Super::BeginPlay();

	//IngameUI
	if (AMBLPlayerController* CT = Cast<AMBLPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		CT->UpdateBossHP(CurrHP, MaxHP);
		OnHPChanged.AddDynamic(CT, &AMBLPlayerController::UpdateBossHP);
	}
	//IngameUI

}

float AMBLBossCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	PlayHitFlash();

	if (CurrHP <= 0.f)
	{
		DeadHandle();
	}

	return DamageAmount;
}

void AMBLBossCharacter::DeadHandle()
{
	Super::DeadHandle();

	if (AMBLGameMode* GameMode = Cast<AMBLGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		GameMode->DeadBoss(); // 보스 사망시
	}

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
	FVector Start = SpawnLocation + FVector(0.f, 0.f, 200.f);
	FVector End = SpawnLocation - FVector(0.f, 0.f, 10000.f);

	FHitResult Hit;
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(PlayerPawn);

	if (GetWorld()->LineTraceSingleByChannel(
		Hit,
		Start,
		End,
		ECC_Visibility,
		TraceParams))
	{
		SpawnLocation.Z = Hit.Location.Z;
	}

	FRotator SpawnRotation = FRotator::ZeroRotator;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;


	AGroundAttack* SkillArea = GetWorld()->SpawnActor<AGroundAttack>(
		GroundAttackClass,
		SpawnLocation,
		SpawnRotation,
		SpawnParams
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
				1.0f,
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
		//GetWorldTimerManager().ClearTimer(DamageTimerHandle);
		//UE_LOG(LogTemp, Warning, TEXT("Player left dectection area."));
	}
}

void AMBLBossCharacter::DamageTick()
{
	if (DamageTarget)
	{
		//데미지 적용
		UGameplayStatics::ApplyDamage(DamageTarget, Attack, GetInstigatorController(), GetInstigator(), UDamageType::StaticClass());
		//UE_LOG(LogTemp, Warning, TEXT("Monster HP : %f / %f"), CurrHP, MaxHP);

		//넉백 적용
		AMBLCharacterBase* Player = Cast<AMBLCharacterBase>(DamageTarget);
		if (Player)
		{
			FVector KnockbackDir = Player->GetActorLocation() - GetActorLocation();
			KnockbackDir.Z = 0.f;
			KnockbackDir.Normalize();

			const float KnockbackStrength = 700.f;
			Player->LaunchCharacter(KnockbackDir * KnockbackStrength, true, true);

			UCharacterMovementComponent* MoveComp = Player->GetCharacterMovement();
			if (MoveComp)
			{
				MoveComp->StopMovementImmediately();
			}
		}
	}
	else
	{
		GetWorldTimerManager().ClearTimer(DamageTimerHandle);
	}
}