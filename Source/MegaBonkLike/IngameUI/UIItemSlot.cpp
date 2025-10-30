#include "IngameUI/UIItemSlot.h"
#include "Components/TextBlock.h"
#include "Item/ItemBase.h"
#include "Item/ItemDataRow.h"

void UUIItemSlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	// 아이템 상세 설명창 추가 (일시 정지 메뉴용)
}

void UUIItemSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	// 아이템 상세 설명창 제거
}

void UUIItemSlot::SetItem(const UItemBase* InItem)
{
	Item = TWeakObjectPtr<const UItemBase>(InItem);
	if (Item.IsValid() == false)
		return;

	if (IsValid(TextItemName) == true)
	{
		TextItemName->SetText(Item->GetData()->ItemName);
	}
	if (IsValid(TextItemLevel) == true)
	{
		TextItemLevel->SetText(FText::FromString(FString::Printf(TEXT("lvl %d"), Item->GetLevel())));
	}
}
