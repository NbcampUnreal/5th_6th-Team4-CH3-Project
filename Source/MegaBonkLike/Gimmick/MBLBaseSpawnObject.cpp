// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/MBLBaseSpawnObject.h"
#include "Components/SceneComponent.h"
//#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AMBLBaseSpawnObject::AMBLBaseSpawnObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComp);

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));	
	CollisionComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	CollisionComp->SetSphereRadius(20.f);
	CollisionComp->SetupAttachment(SceneComp);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComp->SetupAttachment(CollisionComp);

	//CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//CollisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	//CollisionComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	//CollisionComp->SetGenerateOverlapEvents(true);

	DetectionComp = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionComponent"));
	DetectionComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	DetectionComp->SetSphereRadius(300.f);
	DetectionComp->SetupAttachment(SceneComp);

	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AMBLBaseSpawnObject::TemporaryOnPlayerOverlapBegin);
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
		UE_LOG(LogTemp, Warning, TEXT("Player detected"));
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Player detected")));
		//OnObjectActivated(OtherActor);
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

// 임시 함수 지울거임
void AMBLBaseSpawnObject::TemporaryOnPlayerOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		UE_LOG(LogTemp, Warning, TEXT("Gained"));
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Gained")));
		OnObjectActivated(OtherActor);
	}
}

// Called when the game starts or when spawned
void AMBLBaseSpawnObject::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("CollisionComp settings: %s"), *CollisionComp->GetCollisionProfileName().ToString());
	UE_LOG(LogTemp, Warning, TEXT("DetectionComp settings: %s"), *DetectionComp->GetCollisionProfileName().ToString());

}

// Called every frame
void AMBLBaseSpawnObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

