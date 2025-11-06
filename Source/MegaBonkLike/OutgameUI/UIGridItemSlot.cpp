#include "OutgameUI/UIGridItemSlot.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Item/ItemBase.h"
#include "Item/ItemDataRow.h"

void UUIGridItemSlot::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button_Item)
	{
		Button_Item->OnClicked.AddDynamic(this, &UUIGridItemSlot::ButtonClicked);
	}
}

void UUIGridItemSlot::SetItem(TWeakObjectPtr<UItemBase> InItem)
{
	Item = InItem;

	if (Item.IsValid() == false)
	{
		return;
	}

	//  3. 아이템 데이터 존재 여부 확인
	const FItemDataRow* Data = Item->GetData();
	if (Data == nullptr)
	{
		return;
	}

	if (IsValid(TextItemName) == true)
	{
		TextItemName->SetText(Item->GetData()->ItemName);
	}
}

void UUIGridItemSlot::ButtonClicked()
{
	if (Item.IsValid())
	{
		OnSlotClicked.Broadcast(Item.Get()); //알림 보냄
	}
}