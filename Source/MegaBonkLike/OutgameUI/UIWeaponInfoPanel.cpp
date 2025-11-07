#include "OutgameUI/UIWeaponInfoPanel.h"
#include "Components/TextBlock.h"
#include "Item/ItemBase.h"
#include "Item/ItemDataRow.h"

void UUIWeaponInfoPanel::SetWeaponInfo(UItemBase* WeaponItem)
{
    if (!WeaponItem)
    {
        return;
    }

    const FItemDataRow* Data = WeaponItem->GetData();

    if (WeaponName)
    {
        WeaponName->SetText(Data->ItemName);
    }

    if (WeaponDesc)
    {
        WeaponDesc->SetText(Data->ItemDesc);
    }
}
