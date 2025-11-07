#include "IngameUI/UIItemSlot.h"
#include "Components/TextBlock.h"
#include "Item/WeaponItem.h"
#include "Item/TomesItem.h"
#include "Item/MiscItem.h"
#include "Item/ItemDataRow.h"
#include "Components/Image.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"

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

	const FItemDataRow* Data = Item->GetData();
	if (Data == nullptr)
		return;

	if (IsValid(TextItemName) == true)
	{
		//TextItemName->SetText(Data->ItemName);
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
			TextItemLevel->SetText(FText::FromString(FString::Printf(TEXT("x%d"), Misc->GetStackCount())));
		}
	}

	if (IsValid(ImgIcon) == true)
	{
		LoadIcon(Data->ItemIcon);
	}
}

void UUIItemSlot::LoadIcon(const TSoftObjectPtr<UTexture2D>& IconTexture)
{
	if (IconTexture.IsNull())
	{
		ImgIcon->SetVisibility(ESlateVisibility::Collapsed);
		ImgIcon->SetBrushFromTexture(nullptr);
		return;
	}

	ImgIcon->SetVisibility(ESlateVisibility::Visible);
	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
	Streamable.RequestAsyncLoad(IconTexture.ToSoftObjectPath(),
		[this, IconTexture]()
		{
			if (UTexture2D* LoadedIcon = IconTexture.Get())
			{
				ImgIcon->SetBrushFromTexture(LoadedIcon);
			}
		});
}
