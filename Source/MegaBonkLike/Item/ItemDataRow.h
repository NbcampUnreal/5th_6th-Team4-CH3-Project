#pragma once

#include "CoreMinimal.h"
#include "Attribute/AttributeModifier.h"
#include "GameplayTagContainer.h"
#include "Item/ItemEnums.h"
#include "ItemDataRow.generated.h"

class USkill;

USTRUCT(BlueprintType)
struct MEGABONKLIKE_API FItemDataRow : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ItemId;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EItemType ItemType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ItemDesc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FGameplayTag, FAttributeModifier> AttributeModifiers;
};

USTRUCT(BlueprintType)
struct MEGABONKLIKE_API FWeaponItemDataRow : public FItemDataRow
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<USkill> SkillClass;

	FWeaponItemDataRow() { ItemType = EItemType::Weapon; }
};

USTRUCT(BlueprintType)
struct MEGABONKLIKE_API FTomesItemDataRow : public FItemDataRow
{
	GENERATED_BODY()

	FTomesItemDataRow() { ItemType = EItemType::Tomes; }
};

USTRUCT(BlueprintType)
struct MEGABONKLIKE_API FMiscItemDataRow : public FItemDataRow
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMiscItemRarity ItemRarity;

	FMiscItemDataRow() { ItemType = EItemType::Misc; }
};