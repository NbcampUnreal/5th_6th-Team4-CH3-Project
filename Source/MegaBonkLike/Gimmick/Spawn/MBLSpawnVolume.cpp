#include "Gimmick/Spawn/MBLSpawnVolume.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "Kismet/KismetMathLibrary.h"
#include "Gimmick/Spawn/MBLSpawnSubsystem.h"

AMBLSpawnVolume::AMBLSpawnVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	SpawnBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnBox"));
	SpawnBox->SetupAttachment(Scene);

}

void AMBLSpawnVolume::BeginPlay()
{
	Super::BeginPlay();
}

FVector AMBLSpawnVolume::GetRandomEnemySpawnLocation() const
{
	AActor* PlayerActor = GetPlayerInBox();
	if (!PlayerActor) return FVector::ZeroVector;

	const FVector PlayerLocation = PlayerActor->GetActorLocation();

	// 캐릭터 기준 반경 300 ~ 1000M 안의 랜덤 지점 계산
	const float MinRadius = 300.f;
	const float MaxRadius = 1000.f;
	const float Radius = FMath::FRandRange(MinRadius, MaxRadius);
	const float Angle = FMath::FRandRange(0.f, 2 * PI);

	const FVector Offset(FMath::Cos(Angle) * Radius, FMath::Sin(Angle) * Radius, 0.f);
	FVector SpawnLocation = PlayerLocation + Offset;

	return GetValidNavMeshLocation(SpawnLocation, SearchRadius);
}

FVector AMBLSpawnVolume::GetRandomObjectSpawnLocation() const
{
	FVector RandomPoint = UKismetMathLibrary::RandomPointInBoundingBox(
		SpawnBox->Bounds.Origin,
		SpawnBox->Bounds.BoxExtent
	);

	return GetValidNavMeshLocation(RandomPoint, SearchRadius);
}

void AMBLSpawnVolume::SpawnEnemy(TSubclassOf<AActor> EnemyClass)
{
	if (!IsValid(EnemyClass)) return;

	UWorld* World = GetWorld();
	if (!IsValid(World)) return;

	FVector SpawnLocation = GetRandomEnemySpawnLocation();
	//if (SpawnLocation.IsNearlyZero())
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Player not found in spawn box"));
	//	return;
	//}

	AActor* Player = GetPlayerInBox();

	if (!IsValid(Player))
	{
		UE_LOG(LogTemp, Warning, TEXT("No player actor found"));
		return;
	}

	FRotator SpawnRotation = (Player->GetActorLocation() - SpawnLocation).Rotation();

	if (UMBLSpawnSubsystem* Subsystem = World->GetSubsystem<UMBLSpawnSubsystem>())
	{
		Subsystem->SpawnActorAtLocation(EnemyClass, SpawnLocation, SpawnRotation);
	}
}

void AMBLSpawnVolume::SpawnObject(TSubclassOf<AActor> ObjectClass)
{
	if (!ObjectClass) return;

	UWorld* World = GetWorld();
	if (!World) return;

	FVector SpawnLocation = GetRandomObjectSpawnLocation();

	if (SpawnLocation.IsNearlyZero())
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn failed"));
		return;
	}

	FRotator SpawnRotation = FRotator(0.f, FMath::FRandRange(0.f, 360.f), 0.f);

	if (UMBLSpawnSubsystem* Subsystem = World->GetSubsystem<UMBLSpawnSubsystem>())
	{
		Subsystem->SpawnActorAtLocation(ObjectClass, SpawnLocation, SpawnRotation);
	}
}

AActor* AMBLSpawnVolume::GetPlayerInBox() const
{
	UWorld* World = GetWorld();
	if (!World) return nullptr;

	FVector BoxCenter = SpawnBox->GetComponentLocation();
	FVector BoxExtent = SpawnBox->GetScaledBoxExtent();

	TArray<AActor*> Players;
	UGameplayStatics::GetAllActorsWithTag(World, TEXT("Player"), Players);

	AActor* PlayerActor = nullptr;

	for (AActor* Actor : Players)
	{
		if (!Actor) continue;

		FVector Delta = Actor->GetActorLocation() - BoxCenter;

		if (FMath::Abs(Delta.X) <= BoxExtent.X &&
			FMath::Abs(Delta.Y) <= BoxExtent.Y &&
			FMath::Abs(Delta.Z) <= BoxExtent.Z)
		{
			return Actor;
		}
	}

	return nullptr;
}

FVector AMBLSpawnVolume::GetValidNavMeshLocation(const FVector& Location, float Radius) const
{
	UWorld* World = GetWorld();
	if (!World) return FVector::ZeroVector;

	UNavigationSystemV1* NaviSystem = UNavigationSystemV1::GetCurrent(World);
	if (!NaviSystem)
	{
		UE_LOG(LogTemp, Error, TEXT("Navigation system not found"));
		return FVector::ZeroVector;
	}

	FNavLocation ValidLocation;

	bool bFound = NaviSystem->GetRandomPointInNavigableRadius(
		Location,
		Radius,
		ValidLocation
	);

	if (bFound) return ValidLocation.Location;

	UE_LOG(LogTemp, Warning, TEXT("GetValidNavMeshLocation is not valid"));
	return FVector::ZeroVector;
}
