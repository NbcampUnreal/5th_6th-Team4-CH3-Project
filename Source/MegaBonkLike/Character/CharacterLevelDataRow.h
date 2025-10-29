#pragma once

#include "CoreMinimal.h"
#include "CharacterLevelDataRow.generated.h"

USTRUCT()
struct MEGABONKLIKE_API FCharacterLevelDataRow : public FTableRowBase
{
	GENERATED_BODY()	 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxExp;
};
