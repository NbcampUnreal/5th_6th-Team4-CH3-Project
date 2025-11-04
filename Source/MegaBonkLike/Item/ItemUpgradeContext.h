#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Attribute/AttributeModifier.h"
#include "ItemUpgradeContext.generated.h"

USTRUCT(BlueprintType)
struct MEGABONKLIKE_API FItemUpgradeContext
{
	GENERATED_BODY()

	UPROPERTY()
	int32 ItemId;
	UPROPERTY()
	TMap<FGameplayTag, FAttributeModifier> AttributeChanges;
};
