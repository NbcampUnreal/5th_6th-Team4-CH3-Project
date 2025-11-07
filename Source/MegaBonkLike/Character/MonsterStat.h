#pragma once

#include "CoreMinimal.h"
#include "MonsterStat.generated.h"

USTRUCT(BlueprintType)
struct FMonsterStat : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Attack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MoveSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor BodyColor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth;
};
