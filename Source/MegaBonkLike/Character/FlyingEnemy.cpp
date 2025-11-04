#include "Character/FlyingEnemy.h"
#include "Character//MBLPlayerCharacter.h"
#include "AI/MBLAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Game/MBLGameMode.h"
#include "MegaBonkLike.h"

AFlyingEnemy::AFlyingEnemy()
{
	AIControllerClass = AMBLAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	//°¨Áö¿ë Ä¸½¶ÄÄÆ÷
	DamageCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("DamageCollider"));
	DamageCollider->SetupAttachment(GetMesh());
	DamageCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DamageCollider->SetCollisionObjectType(ECC_WorldDynamic);
	DamageCollider->SetCollisionResponseToAllChannels(ECR_Ignore);
	DamageCollider->SetCollisionResponseToChannel(ECC_MBL_PLAYER, ECR_Overlap);
	DamageCollider->SetRelativeLocation(FVector(0.f, 0.f, 90.f));
	DamageCollider->SetCapsuleRadius(60.f);
	DamageCollider->SetCapsuleHalfHeight(90.f);

	DamageCollider->OnComponentBeginOverlap.AddDynamic(this, &AFlyingEnemy::OnDamageColliderBeginOverlap);
	DamageCollider->OnComponentEndOverlap.AddDynamic(this, &AFlyingEnemy::OnDamageColliderEndOverlap);

	bIsDead = false;
	MaxHP = 100;
	CurrHP = MaxHP;
	Attack = 5.f;
}

void AFlyingEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (false == IsPlayerControlled())
	{
		bUseControllerRotationYaw = false;

		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
		GetCharacterMovement()->MaxFlySpeed = 400.f;

	}
}

void AFlyingEnemy::UpdateTrack()
{
	if (!Target.IsValid()) return;

	FVector Start = GetActorLocation();
	FVector End = Target->GetActorLocation();

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		Hit,
		Start,
		End,
		ECC_Visibility,
		Params
	);

	if (bHit)
	{
		FVector AdJustedDiretion = (End - Start).GetSafeNormal();
		AdJustedDiretion.Z += 10.f;
		CurrentDirection = AdJustedDiretion.GetSafeNormal();
	}
	else
	{
		CurrentDirection = (End - Start).GetSafeNormal();
	}
}

void AFlyingEnemy::MoveStep()
{
	if (CurrentDirection.IsNearlyZero()) return;

	FVector NewLocation = GetActorLocation() + CurrentDirection * WalkSpeed * 0.05f;
	SetActorLocation(NewLocation, true);

	FRotator NewRotation = CurrentDirection.Rotation();
	SetActorRotation(NewRotation);
}

void AFlyingEnemy::OnDamageColliderBeginOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 TherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
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
				&AFlyingEnemy::DamageTick,
				0.5f,
				true,
				0.f
			);
		}
	}
}

void AFlyingEnemy::OnDamageColliderEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor == DamageTarget)
	{
		DamageTarget = nullptr;
		GetWorldTimerManager().ClearTimer(DamageTimerHandle);
		//UE_LOG(LogTemp, Warning, TEXT("Player left dectection area."));
	}
}

void AFlyingEnemy::DamageTick()
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

void AFlyingEnemy::DeadHandle()
{
	Super::DeadHandle();
	//if (AMBLGameMode* GameMode = Cast<AMBLGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
	//{
	//	//GameMode->DeadEnemy(); // Àû »ç¸Á½Ã
	//}

	if (bIsDead) return;

	bIsDead = true;

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->DisableMovement();

	FVector Start = GetActorLocation();
	FVector End = Start - FVector(0.f, 0.f, 10000.f);

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	FVector SpawnLocation = GetActorLocation();


	if (GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_Visibility,
		Params))
	{
		SpawnLocation = HitResult.ImpactPoint + FVector(0.f, 0.f, 10.f);
	}

	if (GoldCoin)
	{
		GetWorld()->SpawnActor<AMBLMoneyObject>(GoldCoin, SpawnLocation + FVector(25.f, 0.f, 0.f), FRotator::ZeroRotator);
	}


	if (ExpCoin)
	{
		GetWorld()->SpawnActor<AMBLExpObject>(ExpCoin, SpawnLocation + FVector(-25.f, 0.f, 0.f), FRotator::ZeroRotator);
	}

	Destroy();
	UE_LOG(LogTemp, Warning, TEXT("Died."));
}

void AFlyingEnemy::SetSpeed(float NewSpeed)
{
	GetCharacterMovement()->MaxFlySpeed = NewSpeed;
}
