#include "Character/FlightEnemy.h"
#include "Components/SphereComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "TimerManager.h"
#include "MegaBonkLike.h"
#include "Game/MBLGameMode.h"

AFlightEnemy::AFlightEnemy()
{
	PrimaryActorTick.bCanEverTick = false;
	CurrentDirection = FVector::ZeroVector;

	RootComp = CreateDefaultSubobject<USphereComponent>(TEXT("RootComp"));
	RootComponent = RootComp;

	DamageCollider = CreateDefaultSubobject<USphereComponent>(TEXT("DamageCollider"));
	DamageCollider->SetupAttachment(RootComponent);
	DamageCollider->OnComponentBeginOverlap.AddDynamic(this, &AFlightEnemy::OnDamageOverlapBegin);
	DamageCollider->OnComponentEndOverlap.AddDynamic(this, &AFlightEnemy::OnDamageEndOverlap);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovement"));
	FloatingPawnMovement->MaxSpeed = MoveSpeed;

	CurrHP = MaxHP;
	bCanDamagePlayer = true;
	DamageCooldown = 0.5f;
}

void AFlightEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (!Controller)
	{
		SpawnDefaultController();

		UE_LOG(LogTemp, Warning, TEXT("Controller: %s"), Controller ? *Controller->GetName() : TEXT("nullptr"));

	}

	Target = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	
	GetWorldTimerManager().SetTimer(
		TrackingTimerHandle, 
		this, 
		&AFlightEnemy::UpdateTrack,
		0.1f, 
		true
	);

	GetWorldTimerManager().SetTimer(
		MoveTimerHandle,
		this,
		&AFlightEnemy::MoveStep,
		0.05f,
		true
	);
}

void AFlightEnemy::UpdateTrack()
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

void AFlightEnemy::MoveStep()
{
	if (CurrentDirection.IsNearlyZero()) return;

	FVector NewLocation = GetActorLocation() + CurrentDirection * MoveSpeed * 0.05f;
	SetActorLocation(NewLocation, true);

	FRotator NewRotation = CurrentDirection.Rotation();
	SetActorRotation(NewRotation);
}

void AFlightEnemy::OnDamageOverlapBegin(
	UPrimitiveComponent* OverlappedComp, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, 
	bool bFromSweep, 
	const FHitResult& SweepResult)
{
	if (!OtherActor || !OtherComp) return;

	if (OtherComp->GetCollisionObjectType() == ECC_MBL_PLAYER)
	{
		DamageTarget = OtherActor;

		if (!GetWorldTimerManager().IsTimerActive(DamageTimerHandle))
		{
			GetWorldTimerManager().SetTimer(
				DamageTimerHandle,
				this,
				&AFlightEnemy::DamageTick,
				0.5f,
				true,
				0.f
			);
		}
	}
}

void AFlightEnemy::OnDamageEndOverlap(
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

void AFlightEnemy::DamageTick()
{
	if (DamageTarget)
	{
		UGameplayStatics::ApplyDamage(DamageTarget,Attack,GetController(),this,nullptr);
	}
	else
	{
		GetWorldTimerManager().ClearTimer(DamageTimerHandle);
	}
}

float AFlightEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	CurrHP -= DamageAmount;
	if (CurrHP <= 0.f)
	{
		DeadHandle();
	}
	return DamageAmount;
}

void AFlightEnemy::DeadHandle()
{
	if (AMBLGameMode* GameMode = Cast<AMBLGameMode>(GetWorld()->GetAuthGameMode()))
	{
		GameMode->Dead(this);
	}

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
}
