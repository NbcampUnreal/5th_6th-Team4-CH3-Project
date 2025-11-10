#pragma once

#include "CoreMinimal.h"
#include "IngameUI/PopupBase.h"
#include "PopupItemSelect.generated.h"

class UTextBlock;
class UUIItemSelectOption;
class UButton;
class UPanelWidget;
class UInventoryComponent;
class UUIInventory;
struct FItemSelectOption;

UCLASS()
class MEGABONKLIKE_API UPopupItemSelect : public UPopupBase
{
	GENERATED_BODY()
	
public:
	virtual bool Initialize() override;

	void SetInventory(TWeakObjectPtr<UInventoryComponent> InInventory);

	UFUNCTION()
	void SetOptions(int32 Count);
	UFUNCTION()
	void SelectItem(const FItemSelectOption& Option);
	UFUNCTION()
	void RefreshOptions();
	UFUNCTION()
	void SetRefreshCount(int32 Count);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextOptionName;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPanelWidget> OptionParent;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUIItemSelectOption> UIOptionClass;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> BtnSkip;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> BtnRefresh;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextRefreshCount;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUIInventory> UIInventory;

	UPROPERTY()
	int32 OptionCount = 3;
	UPROPERTY()
	int32 RefreshCount = 3;

	UPROPERTY()
	TWeakObjectPtr<UInventoryComponent> Inventory;
};
