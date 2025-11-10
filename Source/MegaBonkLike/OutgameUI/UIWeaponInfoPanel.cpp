#include "OutgameUI/UIWeaponInfoPanel.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Item/ItemBase.h"
#include "Item/ItemDataRow.h"

void UUIWeaponInfoPanel::SetWeaponInfo(UItemBase* WeaponItem)
{
    if (!WeaponItem)
    {
        return;
    }

    const FItemDataRow* Data = WeaponItem->GetData();

    int32 PanelIndex = Data->ItemId - 100;

    if (WeaponInfoSwitcher)
    {
        if (PanelIndex >= 0 && PanelIndex < WeaponInfoSwitcher->GetNumWidgets())
        {
            WeaponInfoSwitcher->SetActiveWidgetIndex(PanelIndex);
        }
        else
        {
            WeaponInfoSwitcher->SetActiveWidgetIndex(0);
        }
    }

    if (WeaponName)
    {
        WeaponName->SetText(Data->ItemName);
    }

    if (WeaponDesc)
    {
        WeaponDesc->SetText(Data->ItemDesc);
    }
}
