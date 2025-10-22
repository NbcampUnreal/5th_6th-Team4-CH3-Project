#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MBLPlayerInputConfig.generated.h"

class UInputAction;

UCLASS()
class MEGABONKLIKE_API UMBLPlayerInputConfig : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInputAction> IA_Move;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInputAction> IA_Look;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInputAction> IA_Jump;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInputAction> IA_TempTest;
};
