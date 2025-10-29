#include "MBLBaseInteractionObject.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Gimmick/Spawn/MBLSpawnSubsystem.h"
#include "Gimmick/Objects/SpawnObjects/MBLBaseSpawnObject.h"
#include "Components/WidgetComponent.h"
#include "Components/TextBlock.h"

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

	InteractableWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractableWidget"));
	InteractableWidget->SetupAttachment(StaticMeshComp);
	InteractableWidget->SetWidgetSpace(EWidgetSpace::Screen);
	InteractableWidget->SetVisibility(false);
}

void AMBLBaseInteractionObject::BeginPlay()
{
	Super::BeginPlay();

	DetectionComp->OnComponentBeginOverlap.AddDynamic(this, &AMBLBaseInteractionObject::OnPlayerOverlapBegin);
	DetectionComp->OnComponentEndOverlap.AddDynamic(this, &AMBLBaseInteractionObject::OnPlayerOverlapEnd);

	CallOverlap(DetectionComp);
	
	if (InteractableWidget)
	{
		InteractableWidget->SetVisibility(false);
	}

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
			InteractableWidget->SetVisibility(true);
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
			InteractableWidget->SetVisibility(false);
			UE_LOG(LogTemp, Warning, TEXT("Player lost"));
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Player lost")));
		}
	}
}

void AMBLBaseInteractionObject::CallOverlap(UPrimitiveComponent* CollisionComponent)
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

void AMBLBaseInteractionObject::OnObjectActivated(AActor* Activator)
{

}

FName AMBLBaseInteractionObject::GetObejctType() const
{
	return InteractionObjectType;
}

void AMBLBaseInteractionObject::DestroyObject()
{
	Destroy();
}

void AMBLBaseInteractionObject::UpdateWidget()
{
	if (!InteractableWidget) return;

	UUserWidget* InteractableWidgetInstance = InteractableWidget->GetUserWidgetObject();
	if (!InteractableWidgetInstance) return;

	if (UTextBlock* InteractableText = Cast<UTextBlock>(InteractableWidgetInstance->GetWidgetFromName(TEXT("Press"))))
	{
		InteractableText->SetText(FText::FromString(FString::Printf(TEXT("Press 'E'"))));
	}
}
