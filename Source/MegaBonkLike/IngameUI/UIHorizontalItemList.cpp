#include "IngameUI/UIHorizontalItemList.h"
#include "IngameUI/UIItemSlot.h"
#include "Components/PanelWidget.h"

void UUIHorizontalItemList::SetItems(const TArray<UItemBase*> Items)
{
	if (IsValid(ParentPanel) == false)
		return;

	ParentPanel->ClearChildren();

	for (const auto* Item : Items)
	{
		UUIItemSlot* NewItemSlot = CreateWidget<UUIItemSlot>(GetWorld(), ItemSlotClass);
		if (NewItemSlot)
		{
			ParentPanel->AddChild(NewItemSlot);

			NewItemSlot->SetItem(Item);
		}
	}
}
