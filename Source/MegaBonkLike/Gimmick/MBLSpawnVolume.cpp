#include "Gimmick/MBLSpawnVolume.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "Kismet/KismetMathLibrary.h"

AMBLSpawnVolume::AMBLSpawnVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	SpawnBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnBox"));
	SpawnBox->SetupAttachment(Scene);

}

FVector AMBLSpawnVolume::GetRandomEnemySpawnLocation() const
{
	// 예외 처리
	UWorld* World = GetWorld();
	if (!World) return FVector::ZeroVector;

	// 캐릭터 위치 찾기
	FVector BoxCenter = SpawnBox->GetComponentLocation();
	FVector BoxExtent = SpawnBox->GetScaledBoxExtent();

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsWithTag(World, TEXT("Player"), Actors);
	
	AActor* PlayerActor = nullptr;

	for (AActor* Actor : Actors)
	{
		if (!Actor) continue;

		FVector ActorLocation = Actor->GetActorLocation();
		FVector Delta = ActorLocation - BoxCenter;

		if (FMath::Abs(Delta.X) <= BoxExtent.X &&
			FMath::Abs(Delta.Y) <= BoxExtent.Y &&
			FMath::Abs(Delta.Z) <= BoxExtent.Z)
		{
			PlayerActor = Actor;
			break;
		}
	}

	// 예외 처리
	if (!PlayerActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player tag actor not found in spawn box"));
		return FVector::ZeroVector;
	}

	const FVector PlayerLocation = PlayerActor->GetActorLocation();

	// 캐릭터 기준 반경 300 ~ 1000M 안의 랜덤 지점 계산
	const float MinRadius = 300.f;
	const float MaxRadius = 1000.f;
	const float Radius = FMath::FRandRange(MinRadius, MaxRadius);
	const float Angle = FMath::FRandRange(0.f, 2 * PI);

	const FVector Offset(FMath::Cos(Angle) * Radius, FMath::Sin(Angle) * Radius, 0.f);
	FVector SpawnLocation = PlayerLocation + Offset;

	FHitResult HitResult;
	FVector Start = SpawnLocation + FVector(0, 0, 1000);
	FVector End = SpawnLocation + FVector(0, 0, 5000);

	if (World->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility))
	{
		SpawnLocation.Z = HitResult.Location.Z;
	}

	return SpawnLocation;

}

FVector AMBLSpawnVolume::GetRandomObjectSpawnLocation() const
{
	// 예외 처리
	UWorld* World = GetWorld();
	if (!World) return FVector::ZeroVector;

	const FVector BoxOrigin = SpawnBox->Bounds.Origin;
	const FVector BoxExtent = SpawnBox->Bounds.BoxExtent;

	FVector RandomPoint = UKismetMathLibrary::RandomPointInBoundingBox(BoxOrigin, BoxExtent);

	UNavigationSystemV1* NaviSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!NaviSystem)
	{
		UE_LOG(LogTemp, Error, TEXT("Navigation system not found"))
		return FVector::ZeroVector;
	}

	FNavLocation ValidLocation;
	const float SearchRadius = 100.f;

	bool bFound = NaviSystem->GetRandomPointInNavigableRadius(
		RandomPoint, 
		SearchRadius, 
		ValidLocation
	);

	if (bFound) return ValidLocation.Location;

	// 예외 처리
	UE_LOG(LogTemp, Warning, TEXT("Navigation system not found"))
	return FVector::ZeroVector;

}

void AMBLSpawnVolume::SpawnEnemy(TSubclassOf<AActor> EnemyClass)
{
	// 예외 처리
	if (!EnemyClass) return;

	UWorld* World = GetWorld();
	if (!World)return;

	FVector SpawnLocation = GetRandomEnemySpawnLocation();

	if (SpawnLocation.IsNearlyZero())
	{
		UE_LOG(LogTemp, Warning, TEXT("Player not found in spawn box"));
		return;
	}

	TArray<AActor*> PlayerActors;
	UGameplayStatics::GetAllActorsWithTag(World, TEXT("Player"), PlayerActors);

	if (PlayerActors.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No player actor found"));
		return;
	}

	AActor* PlayerActor = PlayerActors[0];
	FVector PlayerLocation = PlayerActor->GetActorLocation();
	FVector DirectionToPlayer = PlayerLocation - SpawnLocation;
	FRotator LookAtRotation = DirectionToPlayer.Rotation();

	GetWorld()->SpawnActor<AActor>(
		EnemyClass,
		SpawnLocation,
		LookAtRotation
	);
}

void AMBLSpawnVolume::SpawnObject(TSubclassOf<AActor> ObjectClass)
{
	// 예외 처리
	if (!ObjectClass) return;

	UWorld* World = GetWorld();
	if (!World)return;

	FVector SpawnLocation = GetRandomObjectSpawnLocation();
	FRotator SpawnRotation = FRotator(0.f, FMath::FRandRange(0.f, 360.f), 0.f);

	if (SpawnLocation.IsNearlyZero())
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn failed"));
		return;
	}

	GetWorld()->SpawnActor<AActor>(
		ObjectClass,
		SpawnLocation,
		SpawnRotation
	);
}

void AMBLSpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	
}


