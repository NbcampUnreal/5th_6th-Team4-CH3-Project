#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ChestRequiredGoldRow.generated.h"

USTRUCT(BlueprintType)
struct MEGABONKLIKE_API FChestRequiredGoldRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Phase;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RequiredGold;
};
