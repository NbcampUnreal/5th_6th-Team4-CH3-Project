#include "Gimmick/MBLBaseInteractionObject.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

AMBLBaseInteractionObject::AMBLBaseInteractionObject()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComp);

	DetectionComp = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionComponent"));
	DetectionComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	DetectionComp->SetSphereRadius(150.f); // 값 조율 필요
	DetectionComp->SetupAttachment(SceneComp);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComp->SetupAttachment(DetectionComp);

	DetectionComp->OnComponentBeginOverlap.AddDynamic(this, &AMBLBaseInteractionObject::OnPlayerOverlapBegin);
	DetectionComp->OnComponentEndOverlap.AddDynamic(this, &AMBLBaseInteractionObject::OnPlayerOverlapEnd);
}

void AMBLBaseInteractionObject::OnPlayerOverlapBegin(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		if (OverlappedComp == DetectionComp)
		{
			UE_LOG(LogTemp, Warning, TEXT("Player can interact this object"));
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Player can interact this object")));
		}
	}
}

void AMBLBaseInteractionObject::OnPlayerOverlapEnd(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		if (OverlappedComp == DetectionComp)
		{
			UE_LOG(LogTemp, Warning, TEXT("Player lost"));
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Player lost")));
		}
	}
}

void AMBLBaseInteractionObject::OnObjectActivated(AActor* Activator)
{
	// 오브젝트 전용 로직 수행후 DestroyObject();
	DestroyObject();
}

FName AMBLBaseInteractionObject::GetObejctType() const
{
	return FName();
}

void AMBLBaseInteractionObject::DestroyObject()
{
	Destroy();
}

void AMBLBaseInteractionObject::BeginPlay()
{
	Super::BeginPlay();

}

void AMBLBaseInteractionObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

