#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "XPBar.generated.h"

class UProgressBar;
class UTextBlock;

UCLASS()
class MEGABONKLIKE_API UXPBar : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void UpdateXP(float CurrentXP, float MaxXP);

	UFUNCTION(BlueprintCallable)
	void UpdateLevel(int32 CurrentLevel);

protected:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> XPProgressBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> XPText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> LevelText;

};
