#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SpawnObjectsSpawnRow.generated.h"

USTRUCT(BlueprintType)
struct FSpawnObjectsSpawnRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ObjectName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> ObjectClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnChance;

};