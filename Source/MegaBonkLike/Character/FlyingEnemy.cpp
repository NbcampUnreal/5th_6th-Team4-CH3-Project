#include "Character/FlyingEnemy.h"
#include "Character//MBLPlayerCharacter.h"
#include "AI/MBLAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
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

	GetCharacterMovement()->MaxFlySpeed = 300.f;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	bIsDead = false;
	MaxHP = 100;
	CurrHP = MaxHP;
	Attack = 5.f;
}

void AFlyingEnemy::BeginPlay()
{
	Super::BeginPlay();

	Target = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	GetCharacterMovement()->SetMovementMode(MOVE_Flying);

	GetWorldTimerManager().SetTimer(
		MoveTimerHandle,
		this,
		&AFlyingEnemy::MoveStep,
		0.05f,
		true
	);

	GetWorldTimerManager().SetTimer(
		TrackTimerHandle,
		this,
		&AFlyingEnemy::UpdateTrack,
		0.1f,
		true
	);
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

	// ¸÷ÀÌ ¶¥°ú ´ê´ÂÁö Ã¼Å©¿ë LineTrace
	FHitResult GroundHit;
	FVector GroundStart = GetActorLocation();
	FVector GroundEnd = GroundStart - FVector(0.f, 0.f, 200.f);

	bool bOnGround = GetWorld()->LineTraceSingleByChannel(
		GroundHit,
		GroundStart,
		GroundEnd,
		ECC_Visibility,
		Params
	);

	// ÇÃ·¹ÀÌ¾î¿Í ¸÷ÀÇ ³ôÀÌ ºñ±³
	const float MyZ =GetActorLocation().Z;
	const float TargetZ = Target->GetActorLocation().Z;
	const float ZDiff = TargetZ - MyZ;

	float FlyStartThreshold = 160.f; 
	float FlyStopThreshold = 0.f;

	if (!bIsFlyingMode && ZDiff >= FlyStartThreshold)
	{
		SetFlyingMode(true);
	}
	else if (bIsFlyingMode && ZDiff < FlyStopThreshold && bOnGround)
	{
		SetFlyingMode(false);
	}

	//UE_LOG(LogTemp, Warning, TEXT("ZDiff: %.2f | bIsFlyingMode: %s"), ZDiff, bIsFlyingMode ? TEXT("true") : TEXT("false"));

}

void AFlyingEnemy::MoveStep()
{
	if (CurrentDirection.IsNearlyZero()) return;

	FVector NewLocation = GetActorLocation() + CurrentDirection * GetCharacterMovement()->MaxFlySpeed * 0.05f;
	SetActorLocation(NewLocation, false);

	FRotator NewRotation = CurrentDirection.Rotation();
	SetActorRotation(NewRotation);
}

void AFlyingEnemy::SetFlyingMode(bool bNewFlying)
{
	bIsFlyingMode = bNewFlying;

	if (AAIController* AICon = Cast<AAIController>(GetController()))
	{
		if (UBlackboardComponent* BB = AICon->GetBlackboardComponent())
		{
			BB->SetValueAsBool(TEXT("IsFlyingMode"), bIsFlyingMode);
		}
	}
	GetCharacterMovement()->SetMovementMode(bIsFlyingMode ? MOVE_Flying : MOVE_Walking);

	UE_LOG(LogTemp, Warning, TEXT("SetFlyingMode called: %s"), bNewFlying ? TEXT("Flying") : TEXT("Walking"));

	if (bIsFlyingMode)
	{

		GetWorldTimerManager().SetTimer(
			MoveTimerHandle,
			this,
			&AFlyingEnemy::MoveStep,
			0.05f,
			true
		);
	}
	else
	{
		GetWorldTimerManager().ClearTimer(MoveTimerHandle);
	}

	if (!GetWorldTimerManager().IsTimerActive(TrackTimerHandle))
	{
		GetWorldTimerManager().SetTimer(
			TrackTimerHandle,
			this,
			&AFlyingEnemy::UpdateTrack,
			0.1f,
			true
		);
	}

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
		//µ¥¹ÌÁö Àû¿ë
		UGameplayStatics::ApplyDamage(DamageTarget, Attack, GetInstigatorController(), GetInstigator(), UDamageType::StaticClass());

		//³Ë¹é Àû¿ë
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

void AFlyingEnemy::DeadHandle()
{
	if (AMBLGameMode* GameMode = Cast<AMBLGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		GameMode->DeadEnemy(); // Àû »ç¸Á½Ã
	}

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

void AFlyingEnemy::SetSpeed(EMBLWaveState Wave)
{
	if (!IsValid(StatDataTable)) return;
	
	if (EMBLWaveState::SetWave < Wave)
	{
		if (Wave > EMBLWaveState::Wave3)
		{
			Wave = EMBLWaveState::Wave3;
		}

		FName RowName(*StaticEnum<EMBLWaveState>()->GetNameStringByValue((int64)Wave));
		FMonsterStat* Monster = StatDataTable->FindRow<FMonsterStat>(RowName, TEXT(""));



		if (!Monster) return;

		GetCharacterMovement()->MaxFlySpeed = Monster->MoveSpeed;
		GetCharacterMovement()->MaxWalkSpeed = (Monster->MoveSpeed) * 2;
	}
}
