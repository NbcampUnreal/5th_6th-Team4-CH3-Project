#include "MonsterAttack/GroundAttack.h"
#include "Components/SphereComponent.h"

AGroundAttack::AGroundAttack()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent")); 
	StaticMeshComponent->SetupAttachment(SceneComponent);

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(SceneComponent);


}

void AGroundAttack::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGroundAttack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

