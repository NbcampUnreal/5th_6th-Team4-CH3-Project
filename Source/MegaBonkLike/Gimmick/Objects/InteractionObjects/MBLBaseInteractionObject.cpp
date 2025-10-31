#include "MBLBaseInteractionObject.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Gimmick/Spawn/MBLSpawnSubsystem.h"
#include "Gimmick/Objects/SpawnObjects/MBLBaseSpawnObject.h"
#include "Components/WidgetComponent.h"
#include "Components/TextBlock.h"
#include "Item/ItemDataRow.h"
// ����ġ ���̺� �߰�

AMBLBaseInteractionObject::AMBLBaseInteractionObject()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComp);

	DetectionComp = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionComponent"));
	DetectionComp->SetCollisionProfileName(TEXT("InteractObject"));
	DetectionComp->SetSphereRadius(150.f); // �� ���� �ʿ�
	DetectionComp->SetupAttachment(SceneComp);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComp->SetupAttachment(DetectionComp);
	StaticMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	InteractableWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractableWidget"));
	InteractableWidget->SetupAttachment(StaticMeshComp);
	InteractableWidget->SetWidgetSpace(EWidgetSpace::Screen);
	InteractableWidget->SetVisibility(false);

}

void AMBLBaseInteractionObject::BeginPlay()
{
	Super::BeginPlay();

	if (!DetectionComp->OnComponentBeginOverlap.IsAlreadyBound(this, &AMBLBaseInteractionObject::OnPlayerOverlapBegin))
	{
		DetectionComp->OnComponentBeginOverlap.AddDynamic(this, &AMBLBaseInteractionObject::OnPlayerOverlapBegin);
	}

	if (!DetectionComp->OnComponentEndOverlap.IsAlreadyBound(this, &AMBLBaseInteractionObject::OnPlayerOverlapEnd))
	{
		DetectionComp->OnComponentEndOverlap.AddDynamic(this, &AMBLBaseInteractionObject::OnPlayerOverlapEnd);
	}

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
	InteractableWidget->SetVisibility(true);
	UE_LOG(LogTemp, Warning, TEXT("Player can interact this object"));
}

void AMBLBaseInteractionObject::OnPlayerOverlapEnd(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	InteractableWidget->SetVisibility(false);
	UE_LOG(LogTemp, Warning, TEXT("Player lost"));
}

void AMBLBaseInteractionObject::CallOverlap(UPrimitiveComponent* CollisionComponent)
{
	if (!CollisionComponent) return;

	CollisionComponent->UpdateOverlaps();

	TArray<AActor*> OverlappingActors;
	CollisionComponent->GetOverlappingActors(OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		FHitResult DummyHit;
		OnPlayerOverlapBegin(CollisionComponent, Actor, nullptr, 0, false, DummyHit);
	}
}

void AMBLBaseInteractionObject::OnObjectActivated(AActor* Activator)
{
	// ��Ÿ ������ ���̺� ������ �ְ�
	// ���̺� 2��
	// ��ȸ�� �� ��� üũ�ϰ�
	// ��Ÿ ������ Ȯ�� ���̺�
	// ����ġ�� ���� ���ϰ�
	// Activator->Cast()Player->FindComponentsByClass<UInventoryComponent>()
	// UInventoryComponent ������->AddOrUpgradeItem(Row->ItemId)
}

FName AMBLBaseInteractionObject::GetObejctType() const
{
	return InteractionObjectType;
}

void AMBLBaseInteractionObject::DestroyObject()
{
	Destroy();
}

