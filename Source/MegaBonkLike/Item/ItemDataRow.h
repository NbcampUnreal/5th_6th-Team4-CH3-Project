#pragma once

#include "CoreMinimal.h"
#include "Attribute/AttributeModifier.h"
#include "GameplayTagContainer.h"
#include "Item/ItemEnums.h"
#include "ItemDataRow.generated.h"

class USkill;

USTRUCT(BlueprintType)
struct FWeaponAttributeEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FGameplayTag AttributeTag;

	UPROPERTY(EditAnywhere)
	float BaseValue;

	UPROPERTY(EditAnywhere)
	FAttributeModifier UpgradeModifier;
};

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
	TSoftObjectPtr<UTexture2D> ItemIcon;
};

USTRUCT(BlueprintType)
struct MEGABONKLIKE_API FWeaponItemDataRow : public FItemDataRow
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<USkill> SkillClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FWeaponAttributeEntry> WeaponAttributeEntry;

	FWeaponItemDataRow() { ItemType = EItemType::Weapon; }
};

USTRUCT(BlueprintType)
struct MEGABONKLIKE_API FTomesItemDataRow : public FItemDataRow
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FGameplayTag, FAttributeModifier> AttributeModifiers;

	FTomesItemDataRow() { ItemType = EItemType::Tomes; }
};

USTRUCT(BlueprintType)
struct MEGABONKLIKE_API FMiscItemDataRow : public FItemDataRow
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemRarity ItemRarity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<USkill> SkillClass;

	FMiscItemDataRow() { ItemType = EItemType::Misc; }
};