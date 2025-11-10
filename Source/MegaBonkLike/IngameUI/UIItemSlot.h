#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIItemSlot.generated.h"

class UTextBlock;
class UItemBase;
class UImage;
class UUIItemSlotInfo;

UCLASS()
class MEGABONKLIKE_API UUIItemSlot : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void SetItem(TWeakObjectPtr<UItemBase> InItem);

	void LoadIcon(const TSoftObjectPtr<UTexture2D>& IconTexture);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextItemName;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextItemLevel;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ImgIcon;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUIItemSlotInfo> UIInfoClass;
	UPROPERTY()
	TObjectPtr<UUIItemSlotInfo> UIInfo;
	UPROPERTY()
	bool bInfoActive;

	UPROPERTY()
	TWeakObjectPtr<const UItemBase> Item;
};
