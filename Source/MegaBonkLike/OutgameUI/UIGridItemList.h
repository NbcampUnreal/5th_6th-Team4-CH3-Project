#pragma once

#include "CoreMinimal.h"
#include "Delegates/DelegateCombinations.h"
#include "Blueprint/UserWidget.h"
#include "UIGridItemList.generated.h"

class UUniformGridPanel;
class UScrollBox;
class UUIGridItemSlot;
class UItemBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemClicked, UItemBase*, ClickedItem);

UCLASS()
class MEGABONKLIKE_API UUIGridItemList : public UUserWidget
{
	GENERATED_BODY()

public:

    void SetItems(const TArray<TWeakObjectPtr<UItemBase>>& Items, int32 Columns = 4);

    UPROPERTY(BlueprintAssignable, Category = "ItemList")
    FOnItemClicked OnItemClicked;

protected:

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UScrollBox> ScrollBox;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UUniformGridPanel> WeaponGridPanel;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<UUIGridItemSlot> ItemSlotClass;

    UFUNCTION()
    void SlotClicked(UItemBase* ClickedItem);
};
