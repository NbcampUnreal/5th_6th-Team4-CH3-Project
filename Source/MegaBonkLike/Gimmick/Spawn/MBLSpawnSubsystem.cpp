#include "Gimmick/Spawn/MBLSpawnSubsystem.h"

AActor* UMBLSpawnSubsystem::SpawnActorAtLocation(
	TSubclassOf<AActor> ActorClass,
	const FVector& Location,
	const FRotator& Rotation
)
{
	if (!ActorClass) return nullptr;

	UWorld* World = GetWorld();
	if (!World) return nullptr;

	return World->SpawnActor<AActor>(ActorClass, Location, Rotation);
}