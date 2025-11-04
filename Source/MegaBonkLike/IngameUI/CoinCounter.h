#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CoinCounter.generated.h"

class UTextBlock;

UCLASS()
class MEGABONKLIKE_API UCoinCounter : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void UpdateCoinCount(int32 CoinCount);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CoinText;
};
