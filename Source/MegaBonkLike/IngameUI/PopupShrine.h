#pragma once

#include "CoreMinimal.h"
#include "IngameUI/PopupBase.h"
#include "PopupShrine.generated.h"

class UInventoryComponent;
class UUIInventory;
class UPanelWidget;
class UButton;
class UUIShrineSelectOption;
struct FShrineOption;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnSelectedShrine, const FShrineOption&);

UCLASS()
class MEGABONKLIKE_API UPopupShrine : public UPopupBase
{
	GENERATED_BODY()
	
public:
	virtual bool Initialize() override;

	void SetInventory(TWeakObjectPtr<UInventoryComponent> InInventory);
	void SetOptions(const TArray<FShrineOption>& Options);

	UFUNCTION()
	void SelectOption(const FShrineOption& SelectedOption);

	FOnSelectedShrine OnSelectedShrine;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUIInventory> UIInventory;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPanelWidget> OptionParent;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> BtnIgnoreOffers;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUIShrineSelectOption> UIOptionClass;

	UPROPERTY()
	TWeakObjectPtr<UInventoryComponent> Inventory;
};
