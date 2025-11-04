#include "Gimmick/Spawn/MBLSpawnSubsystem.h"

AActor* UMBLSpawnSubsystem::SpawnActorAtLocation(
	TSubclassOf<AActor> ActorClass,
	const FVector& Location,
	const FRotator& Rotation
)
{
	if (!IsValid(ActorClass)) return nullptr;

	UWorld* World = GetWorld();
	if (!IsValid(World)) return nullptr;

	return World->SpawnActor<AActor>(ActorClass, Location, Rotation);
}