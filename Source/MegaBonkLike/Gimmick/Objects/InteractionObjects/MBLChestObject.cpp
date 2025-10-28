#include "MBLChestObject.h"
#include "Gimmick/Spawn/MBLSpawnSubsystem.h"

AMBLChestObject::AMBLChestObject()
{
}

void AMBLChestObject::OnObjectActivated(AActor* Activator)
{
	if (!DropItem)
	{
		UE_LOG(LogTemp, Warning, TEXT("DropItem is null"));
		return;
	}

	UWorld* World = GetWorld();
	if (!World) return;

	if (UMBLSpawnSubsystem* Subsystem = World->GetSubsystem<UMBLSpawnSubsystem>())
	{
		Subsystem->SpawnActorAtLocation(DropItem, GetActorLocation(), GetActorRotation());
		DestroyObject();
	}
}
