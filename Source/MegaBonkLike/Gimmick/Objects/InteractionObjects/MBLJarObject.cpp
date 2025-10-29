#include "Gimmick/Objects/InteractionObjects/MBLJarObject.h"
#include "Gimmick/Data/SpawnObjectsSpawnRow.h"
#include "Gimmick/Spawn/MBLSpawnSubsystem.h"

AMBLJarObject::AMBLJarObject()
{
}

void AMBLJarObject::OnObjectActivated(AActor* Activator)
{
	UWorld* World = GetWorld();
	if (!World) return;

	if (FSpawnObjectsSpawnRow* SelectedRow = GetDropItem())
	{
		if (UClass* ActualClass = SelectedRow->ObjectClass.Get())
		{
			if (UMBLSpawnSubsystem* Subsystem = World->GetSubsystem<UMBLSpawnSubsystem>())
			{
				Subsystem->SpawnActorAtLocation(ActualClass, GetActorLocation(), GetActorRotation());
				DestroyObject();
			}
		}
	}
}

FSpawnObjectsSpawnRow* AMBLJarObject::GetDropItem() const
{
	if (!DropTable) return nullptr;

	TArray<FSpawnObjectsSpawnRow*> AllRows;
	static const FString ContextString(TEXT("DropObjectContext"));
	DropTable->GetAllRows(ContextString, AllRows);

	if (AllRows.IsEmpty()) return nullptr;

	float TotalChance = 0.0f;
	for (const FSpawnObjectsSpawnRow* Row : AllRows)
	{
		if (Row)
		{
			TotalChance += Row->SpawnChance;
		}
	}

	const float RandValue = FMath::FRandRange(0.0f, TotalChance);
	float AccumulateChance = 0.0f;

	for (FSpawnObjectsSpawnRow* Row : AllRows)
	{
		AccumulateChance += Row->SpawnChance;
		if (RandValue <= AccumulateChance)
		{
			return Row;
		}
	}

	return nullptr;
}
