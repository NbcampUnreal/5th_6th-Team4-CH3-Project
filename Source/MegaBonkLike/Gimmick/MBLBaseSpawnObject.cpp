#include "Gimmick/MBLBaseSpawnObject.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

AMBLBaseSpawnObject::AMBLBaseSpawnObject()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComp);

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));	
	CollisionComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	CollisionComp->SetSphereRadius(20.f);
	CollisionComp->SetupAttachment(SceneComp);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComp->SetupAttachment(CollisionComp);

	DetectionComp = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionComponent"));
	DetectionComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	DetectionComp->SetSphereRadius(300.f);
	DetectionComp->SetupAttachment(SceneComp);

	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AMBLBaseSpawnObject::OnPlayerOverlapBegin);
	CollisionComp->OnComponentEndOverlap.AddDynamic(this, &AMBLBaseSpawnObject::OnPlayerOverlapEnd);
	
	DetectionComp->OnComponentBeginOverlap.AddDynamic(this, &AMBLBaseSpawnObject::OnPlayerOverlapBegin);
	DetectionComp->OnComponentEndOverlap.AddDynamic(this, &AMBLBaseSpawnObject::OnPlayerOverlapEnd);

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
	DetectionComp->UpdateOverlaps();
	CollisionComp->UpdateOverlaps();

	TArray<AActor*> OverlappingActors;
	DetectionComp->GetOverlappingActors(OverlappingActors);
	
	for (AActor* Actor : OverlappingActors)
	{
		if (Actor && Actor->ActorHasTag("Player"))
		{
			FHitResult DummyHit;
			OnPlayerOverlapBegin(DetectionComp, Actor, nullptr, 0, false, DummyHit);
		}
	}

	OverlappingActors.Empty();
	CollisionComp->GetOverlappingActors(OverlappingActors);
	
	for (AActor* Actor : OverlappingActors)
	{
		if (Actor && Actor->ActorHasTag("Player"))
		{
			FHitResult DummyHit;
			OnPlayerOverlapBegin(CollisionComp, Actor, nullptr, 0, false, DummyHit);
		}
	}
}

void AMBLBaseSpawnObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

