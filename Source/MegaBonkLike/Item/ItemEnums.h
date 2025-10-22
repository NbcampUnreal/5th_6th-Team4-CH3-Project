#pragma once

#include "CoreMinimal.h"
#include "ItemEnums.generated.h"

class USkill;

UENUM()
enum class EItemType : uint8
{
	Weapon,
	Tomes,
	Misc,

	MAX					UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(EItemType, EItemType::MAX);

UENUM()
enum class EItemRarity : uint8
{
	Common,
	Uncommon,
	Rare,
	Epic,
	Legendary
};

UENUM()
enum class EMiscItemRarity : uint8
{
	Common,
	Uncommon,
	Rare,
	Legendary
};