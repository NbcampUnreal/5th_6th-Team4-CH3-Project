#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "MBLSpawnSubsystem.generated.h"

UCLASS()
class MEGABONKLIKE_API UMBLSpawnSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	AActor* SpawnActorAtLocation(
		TSubclassOf<AActor> ActorClass,
		const FVector& Location,
		const FRotator& Rotation
	);
};
