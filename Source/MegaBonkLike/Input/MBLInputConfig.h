#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MBLInputConfig.generated.h"

class UInputAction;

UCLASS()
class MEGABONKLIKE_API UMBLInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInputAction> Move;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInputAction> Look;
};
