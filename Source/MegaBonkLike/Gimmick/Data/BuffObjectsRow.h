#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Attribute/AttributeModifier.h"
#include "GameplayTagContainer.h"
#include "BuffObjectsRow.generated.h"

USTRUCT(BlueprintType)
struct MEGABONKLIKE_API FBuffObjectsRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TMap<FGameplayTag, FAttributeModifier> Type;
};
