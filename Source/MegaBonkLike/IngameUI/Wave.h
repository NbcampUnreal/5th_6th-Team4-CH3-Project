#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Wave.generated.h"

class UTextBlock;

UCLASS()
class MEGABONKLIKE_API UWave : public UUserWidget
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable)
	void UpdateWaveText(int32 CurrentWave, int32 MaxWave);

protected:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> WaveText;
};
