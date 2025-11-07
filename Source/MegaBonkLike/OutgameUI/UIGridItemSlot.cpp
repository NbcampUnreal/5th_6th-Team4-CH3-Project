#include "OutgameUI/UIGridItemSlot.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Item/ItemBase.h"
#include "Item/ItemDataRow.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"

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

	const FItemDataRow* Data = Item->GetData();
	if (Data == nullptr)
	{
		return;
	}

	if (IsValid(TextItemName) == true)
	{
		TextItemName->SetText(Item->GetData()->ItemName);
	}

	if (IsValid(ImgIcon) == true)
	{
		LoadIcon(Data->ItemIcon);
	}
}

void UUIGridItemSlot::ButtonClicked()
{
	if (Item.IsValid())
	{
		OnSlotClicked.Broadcast(Item.Get());
	}
}

void UUIGridItemSlot::LoadIcon(const TSoftObjectPtr<UTexture2D>& IconTexture)
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