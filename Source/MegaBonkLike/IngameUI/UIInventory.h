#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIInventory.generated.h"

class UUIHorizontalItemList;
class UInventoryComponent;

UCLASS()
class MEGABONKLIKE_API UUIInventory : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetInventory(TWeakObjectPtr<UInventoryComponent> InInventory);

	UFUNCTION()
	void UpdateItems();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUIHorizontalItemList> WeaponList;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUIHorizontalItemList> TomesList;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUIHorizontalItemList> MiscList;

	UPROPERTY()
	TWeakObjectPtr <UInventoryComponent> Inventory;
};
