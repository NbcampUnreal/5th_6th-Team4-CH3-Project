#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Timer.generated.h"

class UTextBlock;

UCLASS()
class MEGABONKLIKE_API UTimer : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
	void UpdateTimer(float RemainingTime);

protected:

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TimerText;

};
