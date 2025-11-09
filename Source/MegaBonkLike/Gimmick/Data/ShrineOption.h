#pragma once

#include "CoreMinimal.h"
#include "Item/ItemEnums.h"
#include "GameplayTagContainer.h"
#include "Attribute/AttributeModifier.h"
#include "ShrineOption.generated.h"

USTRUCT(BlueprintType)
struct MEGABONKLIKE_API FShrineOption
{
	GENERATED_BODY()

	UPROPERTY()
	EItemRarity Rarity;
	UPROPERTY()
	FGameplayTag Tag;
	UPROPERTY()
	FAttributeModifier Modifier;
};
