#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BossHPBar.generated.h"

class UProgressBar;

UCLASS()
class MEGABONKLIKE_API UBossHPBar : public UUserWidget
{
	GENERATED_BODY()
public:
	
	UFUNCTION(BlueprintCallable)
	void UpdateBossHP(float CurrentHP, float MaxHP);

protected:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> BossHPProgressBar;
};
