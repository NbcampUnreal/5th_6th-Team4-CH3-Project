#include "MBLBaseSpawnObject.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "TimerManager.h"

AMBLBaseSpawnObject::AMBLBaseSpawnObject()
	: RotationSpeed(90.f)
	, UpdateRotation(0.02f)
	, TargetActor(nullptr)
	, BaseSpeed(1000.f)
	, UpdateInterval(0.05f)
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComp);

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));	
	CollisionComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	CollisionComp->SetSphereRadius(20.f); // 임시 값
	CollisionComp->SetupAttachment(SceneComp);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComp->SetupAttachment(CollisionComp);

	DetectionComp = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionComponent"));
	DetectionComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	DetectionComp->SetSphereRadius(300.f); // 임시 값
	DetectionComp->SetupAttachment(SceneComp);

	ProjectileComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileComp->ProjectileGravityScale = 0.f;
	ProjectileComp->bShouldBounce = false;
	ProjectileComp->bRotationFollowsVelocity = true;
}

void AMBLBaseSpawnObject::BeginPlay()
{
	Super::BeginPlay();

	if (!CollisionComp->OnComponentBeginOverlap.IsAlreadyBound(this, &AMBLBaseSpawnObject::OnPlayerOverlapBegin))
	{
		CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AMBLBaseSpawnObject::OnPlayerOverlapBegin);
	}
	if (!CollisionComp->OnComponentEndOverlap.IsAlreadyBound(this, &AMBLBaseSpawnObject::OnPlayerOverlapEnd))
	{
		CollisionComp->OnComponentEndOverlap.AddDynamic(this, &AMBLBaseSpawnObject::OnPlayerOverlapEnd);
	}
	if (!DetectionComp->OnComponentBeginOverlap.IsAlreadyBound(this, &AMBLBaseSpawnObject::OnPlayerOverlapBegin))
	{
		DetectionComp->OnComponentBeginOverlap.AddDynamic(this, &AMBLBaseSpawnObject::OnPlayerOverlapBegin);
	}
	if (!DetectionComp->OnComponentEndOverlap.IsAlreadyBound(this, &AMBLBaseSpawnObject::OnPlayerOverlapEnd))
	{
		DetectionComp->OnComponentEndOverlap.AddDynamic(this, &AMBLBaseSpawnObject::OnPlayerOverlapEnd);
	}

	// 스폰시 이미 캐릭터와 겹쳐있을 경우를 위한 오버랩 함수 수동 호출
	CallOverlap(DetectionComp);
	CallOverlap(CollisionComp);

	GetWorldTimerManager().SetTimer(
		RotationTimerHandle,
		this,
		&AMBLBaseSpawnObject::RotationObject,
		UpdateRotation,
		true
	);

	GetWorldTimerManager().SetTimer(
		ChaseTimerHandle,
		this,
		&AMBLBaseSpawnObject::ChaseToPlayer,
		UpdateInterval,
		true
	);
}

void AMBLBaseSpawnObject::OnPlayerOverlapBegin(
	UPrimitiveComponent* OverlappedComp, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, 
	bool bFromSweep, const 
	FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		if (OverlappedComp == DetectionComp)
		{
			UE_LOG(LogTemp, Warning, TEXT("Player detected"));
			TargetActor = OtherActor;
		}
		else if (OverlappedComp == CollisionComp)
		{
			UE_LOG(LogTemp, Warning, TEXT("Gained"));
			OnObjectActivated(OtherActor);
		}
	}
}

void AMBLBaseSpawnObject::OnPlayerOverlapEnd(
	UPrimitiveComponent* OverlappedComp, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex)
{
}

void AMBLBaseSpawnObject::CallOverlap(UPrimitiveComponent* CollisionComponent)
{
	if (!CollisionComponent) return;

	CollisionComponent->UpdateOverlaps();

	TArray<AActor*> OverlappingActors;
	CollisionComponent->GetOverlappingActors(OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		if (Actor && Actor->ActorHasTag("Player"))
		{
			FHitResult DummyHit;
			OnPlayerOverlapBegin(CollisionComponent, Actor, nullptr, 0, false, DummyHit);
		}
	}
}

void AMBLBaseSpawnObject::OnObjectActivated(AActor* Activator)
{
	UE_LOG(LogTemp, Warning, TEXT("BaseSpawnObject OnObjectActivated() Called"));
}

FName AMBLBaseSpawnObject::GetObejctType() const
{
	return SpawnObjectType;
}

void AMBLBaseSpawnObject::DestroyObject()
{
	Destroy();
}

void AMBLBaseSpawnObject::RotationObject()
{
	FRotator CurrentRotation = StaticMeshComp->GetRelativeRotation();
	CurrentRotation.Yaw += RotationSpeed * UpdateRotation;

	StaticMeshComp->SetRelativeRotation(CurrentRotation);
}

void AMBLBaseSpawnObject::ChaseToPlayer()
{
	if (!IsValid(TargetActor))
	{
		TargetActor = nullptr;
		return;
	}

	if (!ProjectileComp) return;

	FVector ObjectLocation = GetActorLocation();
	FVector TargetLocation = TargetActor->GetActorLocation();
	FVector Direction = (TargetLocation - ObjectLocation).GetSafeNormal();

	float Distance = FVector::Dist(ObjectLocation, TargetLocation);
	float DitectionRadius = DetectionComp->GetScaledSphereRadius();

	float DistanceRatio = FMath::Clamp(Distance / DitectionRadius, 0.0f, 1.0f);

	float Acceleration = FMath::Lerp(1.0f, 3.5f, DistanceRatio);
	float CurrentSpeed = FMath::Min(BaseSpeed + Acceleration * 50.0f * GetWorld()->GetDeltaSeconds(), BaseSpeed * 5.0f);

	if (Distance < 100.0f)
	{
		CurrentSpeed = FMath::Max(CurrentSpeed * 0.8f, BaseSpeed);
	}

	ProjectileComp->Velocity = Direction * CurrentSpeed;
	
}

