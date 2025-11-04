#include "Character/MBLNonPlayerCharacter.h"
#include "Character//MBLPlayerCharacter.h"
#include "AI/MBLAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Game/MBLGameMode.h"
#include "MegaBonkLike.h"


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

	//감지용 캡슐컴포
	DamageCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("DamageCollider"));
	DamageCollider->SetupAttachment(GetMesh());
	DamageCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DamageCollider->SetCollisionObjectType(ECC_WorldDynamic);
	DamageCollider->SetCollisionResponseToAllChannels(ECR_Ignore);
	DamageCollider->SetCollisionResponseToChannel(ECC_MBL_PLAYER, ECR_Overlap);
	DamageCollider->SetRelativeLocation(FVector(0.f, 0.f, 90.f));
	DamageCollider->SetCapsuleRadius(60.f);
	DamageCollider->SetCapsuleHalfHeight(90.f);

	DamageCollider->OnComponentBeginOverlap.AddDynamic(this, &AMBLNonPlayerCharacter::OnDamageColliderBeginOverlap);
	DamageCollider->OnComponentEndOverlap.AddDynamic(this, &AMBLNonPlayerCharacter::OnDamageColliderEndOverlap);

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.5f;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);

	GetCharacterMovement()->bUseRVOAvoidance = true;
	GetCharacterMovement()->AvoidanceWeight = 1.f;

	bIsDead = false;
	MaxHP = 100;
	CurrHP = MaxHP;
	Attack = 5.f;
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


void AMBLNonPlayerCharacter::DeadHandle()
{
	Super::DeadHandle();
	// if(AMBLGameMode* GameMode = Cast<AMBLGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
	//{
	//	//GameMode->DeadEnemy(); // 적 사망시
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

void AMBLNonPlayerCharacter::OnDamageColliderBeginOverlap(
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
				&AMBLNonPlayerCharacter::DamageTick,
				0.5f,
				true,
				0.f
			);
		}
	}
}

void AMBLNonPlayerCharacter::OnDamageColliderEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor ==DamageTarget)
	{
		DamageTarget = nullptr;
		GetWorldTimerManager().ClearTimer(DamageTimerHandle);
		//UE_LOG(LogTemp, Warning, TEXT("Player left dectection area."));
	}
}

void AMBLNonPlayerCharacter::DamageTick()
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

/*테스트용 코드
void AMBLNonPlayerCharacter::KillSelf()
{
	DeadHandle();
}*/