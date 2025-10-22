#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MBLPlayerController.generated.h"

class UInputMappingContext;

UCLASS()
class MEGABONKLIKE_API AMBLPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInputMappingContext> IMC_Base;
};
