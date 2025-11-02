#pragma once

#include "CoreMinimal.h"
#include "Item/ItemEnums.h"
#include "ItemRarityDataRow.generated.h"


USTRUCT(BlueprintType)
struct MEGABONKLIKE_API FItemRarityDataRow : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemRarity ItemRarity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Multiplier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AppearanceRate;
};