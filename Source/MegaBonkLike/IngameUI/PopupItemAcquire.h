#pragma once

#include "CoreMinimal.h"
#include "IngameUI/PopupBase.h"
#include "Item/ItemSelectOption.h"
#include "PopupItemAcquire.generated.h"

class UTextBlock;
class UButton;
class UImage;
class UInventoryComponent;
class UUIInventory;

UCLASS()
class MEGABONKLIKE_API UPopupItemAcquire : public UPopupBase
{
	GENERATED_BODY()
	
public:
	virtual bool Initialize() override;

	void SetInventory(TWeakObjectPtr<UInventoryComponent> InInventory);
	void SetOption();
	UFUNCTION()
	void TakeItem();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextRarity;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextName;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextDesc;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ImgIcon;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> BtnSkip;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> BtnTake;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUIInventory> UIInventory;

	UPROPERTY(EditAnywhere)
	TMap<EItemRarity, FLinearColor> ColorTextRarity;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UDataTable> MiscTable;
	UPROPERTY()
	TWeakObjectPtr<UInventoryComponent> Inventory;

	FItemSelectOption Option;
};
