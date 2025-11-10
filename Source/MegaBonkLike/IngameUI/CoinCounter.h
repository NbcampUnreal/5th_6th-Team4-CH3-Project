#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CoinCounter.generated.h"

class UTextBlock;
class UImage;

UCLASS()
class MEGABONKLIKE_API UCoinCounter : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void UpdateCoinCount(int32 CoinCount);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr <UImage> CoinImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CoinCountText;
};
