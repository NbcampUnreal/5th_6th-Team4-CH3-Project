
#include "OutgameUI/UIGridItemList.h"
#include "OutgameUI/UIGridItemSlot.h"
#include "Components/ScrollBox.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"

void UUIGridItemList::SetItems(const TArray<TWeakObjectPtr<UItemBase>>& Items, int32 Columns)
{
    if (!IsValid(WeaponGridPanel) || !IsValid(ScrollBox))
        return;

    WeaponGridPanel->ClearChildren();

    const int32 NumItems = Items.Num();
    const int32 Rows = FMath::CeilToInt((float)NumItems / Columns);

    for (int32 i = 0; i < NumItems; ++i)
    {
        UUIGridItemSlot* NewItemSlot = CreateWidget<UUIGridItemSlot>(GetWorld(), ItemSlotClass);
        if (NewItemSlot)
        {
            const int32 Row = i / Columns;
            const int32 Col = i % Columns;

            UUniformGridSlot* GridSlot = WeaponGridPanel->AddChildToUniformGrid(NewItemSlot, Row, Col);
            GridSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
            GridSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);

            NewItemSlot->SetItem(Items[i]);

            NewItemSlot->OnSlotClicked.AddDynamic(this, &UUIGridItemList::SlotClicked);
        }
    }
}

void UUIGridItemList::SlotClicked(UItemBase* ClickedItem)
{
    OnItemClicked.Broadcast(ClickedItem); //위젯으로 신호 전달
}
