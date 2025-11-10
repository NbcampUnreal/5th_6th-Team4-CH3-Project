#include "IngameUI/UIItemSlotInfo.h"
#include "Item/WeaponItem.h"
#include "Item/TomesItem.h"
#include "Item/MiscItem.h"
#include "Item/ItemDataRow.h"
#include "Components/TextBlock.h"

void UUIItemSlotInfo::SetInfo(TWeakObjectPtr<const UItemBase> InItem)
{
	if (InItem.IsValid() == false)
		return;

	const FItemDataRow* Data = InItem->GetData();
	if (Data == nullptr)
		return;

	if (IsValid(TextItemName) == true)
	{
		TextItemName->SetText(Data->ItemName);

		if (Data->ItemType == EItemType::Misc)
		{
			const FMiscItemDataRow* MiscData = (FMiscItemDataRow*)Data;
			TextItemName->SetColorAndOpacity(FSlateColor(ColorTextRarity[MiscData->ItemRarity]));
		}
		else
		{
			TextItemName->SetColorAndOpacity(FSlateColor(FLinearColor::White));
		}
	}

	if (IsValid(TextItemDesc) == true)
	{
		TextItemDesc->SetText(Data->ItemDesc);
	}
}
