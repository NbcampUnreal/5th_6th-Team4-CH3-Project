#pragma once

#include "CoreMinimal.h"
#include "MonsterInfo.generated.h"

USTRUCT(BlueprintType)
struct FMonsterInfo : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> MonsterClass;

};
