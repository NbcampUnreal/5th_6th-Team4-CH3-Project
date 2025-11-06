#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIGridItemSlot.generated.h"

class UTextBlock;
class UItemBase;
class UButton;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotClicked, UItemBase*, ClickedItem);

UCLASS()
class MEGABONKLIKE_API UUIGridItemSlot : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetItem(TWeakObjectPtr<UItemBase> InItem);

	UPROPERTY(BlueprintAssignable, Category = "Item")
	FOnSlotClicked OnSlotClicked;

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextItemName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Item;

	UPROPERTY()
	TWeakObjectPtr<UItemBase> Item;

	UFUNCTION()
	void ButtonClicked();
};
