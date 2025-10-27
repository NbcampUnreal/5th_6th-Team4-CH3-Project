#include "Gimmick/MBLBaseSpawnObject.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "TimerManager.h"
//#include "Kismet/GameplayStatics.h"

AMBLBaseSpawnObject::AMBLBaseSpawnObject()
	: TargetActor(nullptr)
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

	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AMBLBaseSpawnObject::OnPlayerOverlapBegin);
	CollisionComp->OnComponentEndOverlap.AddDynamic(this, &AMBLBaseSpawnObject::OnPlayerOverlapEnd);
	
	DetectionComp->OnComponentBeginOverlap.AddDynamic(this, &AMBLBaseSpawnObject::OnPlayerOverlapBegin);
	DetectionComp->OnComponentEndOverlap.AddDynamic(this, &AMBLBaseSpawnObject::OnPlayerOverlapEnd);

	ProjectileComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileComp->ProjectileGravityScale = 0.f;
	ProjectileComp->bShouldBounce = false;
	ProjectileComp->bRotationFollowsVelocity = true;
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
			TargetActor = OtherActor;
			UE_LOG(LogTemp, Warning, TEXT("Player detected"));
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Player detected")));
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
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Gained")));
}

FName AMBLBaseSpawnObject::GetObejctType() const
{
	return SpawnObjectType;
}

void AMBLBaseSpawnObject::DestroyObject()
{
	Destroy();
}

void AMBLBaseSpawnObject::BeginPlay()
{
	Super::BeginPlay();
	// 스폰시 이미 캐릭터와 겹쳐있을 경우를 위한 오버랩 함수 수동 호출
	CallOverlap(DetectionComp);
	CallOverlap(CollisionComp);

	GetWorldTimerManager().SetTimer(
		ChaseTimerHandle,
		this,
		&AMBLBaseSpawnObject::ChaseToPlayer,
		UpdateInterval,
		true);
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

	float AccelerationRange = FMath::Clamp(Distance / DitectionRadius, 0.3f, 1.0f);
	float CurrentSpeed = BaseSpeed * AccelerationRange;

	ProjectileComp->Velocity = Direction * CurrentSpeed;
	
}


