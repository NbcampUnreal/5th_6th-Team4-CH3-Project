#include "IngameUI/UIHorizontalItemList.h"
#include "IngameUI/UIItemSlot.h"
#include "Components/WrapBox.h"

void UUIHorizontalItemList::SetItems(const TArray<TWeakObjectPtr<UItemBase>>& Items)
{
	if (IsValid(ParentPanel) == false)
		return;

	ParentPanel->ClearChildren();

	for (const auto& Item : Items)
	{
		UUIItemSlot* NewItemSlot = CreateWidget<UUIItemSlot>(GetWorld(), ItemSlotClass);
		if (NewItemSlot)
		{
			ParentPanel->AddChild(NewItemSlot);

			NewItemSlot->SetItem(Item);
		}
	}
}
void UUIHorizontalItemList::SetHorizontalAlignment(EHorizontalAlignment Alignment)
{
	ParentPanel->SetHorizontalAlignment(Alignment);
}