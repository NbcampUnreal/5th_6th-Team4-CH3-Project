#include "Character/MBLEliteMonster.h"
#include "AI/MBLAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Game/MBLGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/Monster/EliteEnemyAnimInstance.h"
#include "MegaBonkLike.h"

AMBLEliteMonster::AMBLEliteMonster()
{
	AIControllerClass = AMBLAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	float CharacterHalfHeight = 90.f;
	float CharacterRadius = 40.f;

	GetCapsuleComponent()->InitCapsuleSize(CharacterRadius, CharacterHalfHeight);

	FVector PivotPosition(0.f, 0.f, -CharacterHalfHeight);
	FRotator PivotRotation(0.f, -90.f, 0.f);
	GetMesh()->SetRelativeLocationAndRotation(PivotPosition, PivotRotation);

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

	DamageCollider->OnComponentBeginOverlap.AddDynamic(this, &AMBLEliteMonster::OnDamageColliderBeginOverlap);
	DamageCollider->OnComponentEndOverlap.AddDynamic(this, &AMBLEliteMonster::OnDamageColliderEndOverlap);

	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.5f;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);

	GetCharacterMovement()->bUseRVOAvoidance = true;
	GetCharacterMovement()->AvoidanceWeight = 1.f;

	bUseControllerRotationYaw = false;

	bIsDead = false;
	MaxHP = 500;
	CurrHP = MaxHP;
	Attack = 5.f;
}

void AMBLEliteMonster::BeginPlay()
{
    Super::BeginPlay();

/*	if (GetController() == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("AIController OO"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AIController XX"));

	}*/

	GetWorld()->GetTimerManager().SetTimer(
		WeaponEquip,
		this,
		&AMBLEliteMonster::SpawnWeapon,
		3.0f,
		false);

}

void AMBLEliteMonster::CheckAttackCondition()
{
	AAIController* MyAI = Cast<AAIController>(GetController());
	if (MyAI)
	{
		UBlackboardComponent* BB = MyAI->GetBlackboardComponent();
		if (BB)
		{
			DamageTarget = Cast<AActor>(BB->GetValueAsObject(TEXT("TargetCharacter")));
		}
	}
	
	if (!DamageTarget || bIsAiming || !bCanAttack) return;

	float Distance = FVector::Dist(GetActorLocation(), DamageTarget->GetActorLocation());
	if (Distance <= AttackRange)
	{
		bIsAiming = true;
		bCanAttack = false;

		MyAI->StopMovement();
		StartAiming();
	}
}

void AMBLEliteMonster::StartAttack()
{
}

void AMBLEliteMonster::EndAttack()
{
}

void AMBLEliteMonster::ResetAttack()
{
}

void AMBLEliteMonster::StartAiming()
{
	/*if (AimMontage)
	{
		PlayAnimMontage(AimMontage);
	}

	GetWorldTimerManager().SetTimer(
		AimTimer,
		this,
		&A)*/
}

void AMBLEliteMonster::SpawnWeapon()
{
	if (!WeaponClass) return;

	FActorSpawnParameters SpawnParams;
	FVector SpawnLocation = GetMesh()->GetSocketLocation(TEXT("hand_rSocket"));
	FRotator SpawnRotation = GetMesh()->GetSocketRotation(TEXT("hand_rSocket"));
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	EquippedWeapon = GetWorld()->SpawnActor<AActor>(WeaponClass,SpawnLocation, SpawnRotation, SpawnParams);
	if (EquippedWeapon)
	{
		EquippedWeapon->SetActorEnableCollision(false);
		EquippedWeapon->AttachToComponent(
			GetMesh(),
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			FName("hand_rSocket")
		);
	}
}

void AMBLEliteMonster::OnDamageColliderBeginOverlap(
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
				&AMBLEliteMonster::DamageTick,
				0.5f,
				true,
				0.f
			);
		}
	}
}

void AMBLEliteMonster::OnDamageColliderEndOverlap(
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

void AMBLEliteMonster::HandleOnCheckAttack()
{
	UKismetSystemLibrary::PrintString(this, TEXT("HandleOnCheckHit()"));
}

void AMBLEliteMonster::DamageTick()
{
	if (DamageTarget)
	{
		//µ¥¹ÌÁö Àû¿ë
		UGameplayStatics::ApplyDamage(DamageTarget, Attack, GetInstigatorController(), GetInstigator(), UDamageType::StaticClass());
		//UE_LOG(LogTemp, Warning, TEXT("Monster HP : %f / %f"), CurrHP, MaxHP);

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

void AMBLEliteMonster::DeadHandle()
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
	UE_LOG(LogTemp, Warning, TEXT("Elite Died."));
}