#include "IngameUI/UIHorizontalItemList.h"
#include "IngameUI/UIItemSlot.h"
#include "Components/WrapBox.h"

void UUIHorizontalItemList::SetItems(const TArray<TWeakObjectPtr<UItemBase>>& Items, int32 DefaultSlotCount)
{
	if (IsValid(ParentPanel) == false)
		return;

	ParentPanel->ClearChildren();

	int32 SlotCount = FMath::Max(Items.Num(), DefaultSlotCount);
	for (int32 i = 0; i < SlotCount; ++i)
	{
		UUIItemSlot* NewItemSlot = CreateWidget<UUIItemSlot>(GetWorld(), ItemSlotClass);
		if (NewItemSlot)
		{
			ParentPanel->AddChild(NewItemSlot);

			if (i < Items.Num())
			{
				NewItemSlot->SetItem(Items[i]);
			}
		}
	}
}
void UUIHorizontalItemList::SetHorizontalAlignment(EHorizontalAlignment Alignment)
{
	ParentPanel->SetHorizontalAlignment(Alignment);
}