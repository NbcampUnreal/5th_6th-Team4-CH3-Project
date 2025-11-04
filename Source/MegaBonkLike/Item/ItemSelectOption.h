#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Item/ItemEnums.h"
#include "Attribute/AttributeModifier.h"
#include "ItemSelectOption.generated.h"

USTRUCT(BlueprintType)
struct FAttributeComparison
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FGameplayTag AttributeTag;
    UPROPERTY(BlueprintReadOnly)
    EAttributeModifierType ModifierType;
    UPROPERTY(BlueprintReadOnly)
    float CurrentValue = 0.0f;
    UPROPERTY(BlueprintReadOnly)
    float NewValue = 0.0f;
    UPROPERTY(BlueprintReadOnly)
    float DeltaValue = 0.0f;
};

USTRUCT(BlueprintType)
struct MEGABONKLIKE_API FItemSelectOption
{
	GENERATED_BODY()

    UPROPERTY()
    bool bIsNewItem = false;
    UPROPERTY()
	int32 ItemId;
    UPROPERTY()
    int32 Level;
    UPROPERTY()
    EItemType ItemType;
    UPROPERTY()
	EItemRarity Rarity;
    UPROPERTY()
	TArray<FAttributeComparison> AttributeChanges;
};
