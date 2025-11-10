#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KillCounter.generated.h"

class UTextBlock;
class UImage;

UCLASS()
class MEGABONKLIKE_API UKillCounter : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
	void UpdateKillCounter(int32 KillCount);


protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr <UImage> KillCounterImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> KillCounterText;
};


