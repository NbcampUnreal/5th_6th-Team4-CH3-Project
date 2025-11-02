#include "IngameUI/UIItemSlot.h"
#include "Components/TextBlock.h"
#include "Item/WeaponItem.h"
#include "Item/TomesItem.h"
#include "Item/MiscItem.h"
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

void UUIItemSlot::SetItem(TWeakObjectPtr<UItemBase> InItem)
{
	Item = InItem;
	if (Item.IsValid() == false)
		return;

	if (IsValid(TextItemName) == true)
	{
		TextItemName->SetText(Item->GetData()->ItemName);
	}

	if (IsValid(TextItemLevel) == true)
	{
		if (const UWeaponItem* Weapon = Cast<const UWeaponItem>(Item))
		{
			TextItemLevel->SetText(FText::FromString(FString::Printf(TEXT("lvl %d"), Weapon->GetLevel())));
		}
		else if (const UTomesItem* Tomes = Cast<const UTomesItem>(Item))
		{
			TextItemLevel->SetText(FText::FromString(FString::Printf(TEXT("lvl %d"), Tomes->GetLevel())));
		}
		else if (const UMiscItem* Misc = Cast<const UMiscItem>(Item))
		{
			TextItemLevel->SetText(FText::FromString(FString::Printf(TEXT("%d"), Misc->GetStackCount())));
		}
	}
}
