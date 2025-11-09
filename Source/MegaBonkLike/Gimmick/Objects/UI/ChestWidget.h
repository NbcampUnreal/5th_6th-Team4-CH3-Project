#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChestWidget.generated.h"

class UTextBlock;
class UImage;

UCLASS()
class MEGABONKLIKE_API UChestWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	FTimerHandle MessageTimerHandle;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UImage* LockIcon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* DenyText;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UImage* PressKeyIcon;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* GoldText;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (BindWidget))
	UImage* GoldIcon;

	virtual void NativeOnInitialized() override;
	void ViewDenyMessage();
	void HideMessage();
	void SetGoldText(float RequiredGold);
	
};
