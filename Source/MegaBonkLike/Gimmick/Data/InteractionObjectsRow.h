#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "InteractionObjectsRow.generated.h"

USTRUCT(BlueprintType)
struct FInteractionObjectsRow : public FTableRowBase
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
