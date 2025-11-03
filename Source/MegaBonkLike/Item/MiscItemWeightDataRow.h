#pragma once

#include "CoreMinimal.h"
#include "Item/ItemEnums.h"
#include "MiscItemWeightDataRow.generated.h"

USTRUCT(BlueprintType)
struct MEGABONKLIKE_API FMiscItemWeightDataRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMiscItemRarity ItemRarity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Weight;
};