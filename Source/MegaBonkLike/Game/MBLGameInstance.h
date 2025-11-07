#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MBLGameInstance.generated.h"


UCLASS()
class MEGABONKLIKE_API UMBLGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	int32 SelectedWeaponId;
};
