#include "IngameUI/PopupItemAcquire.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Character/InventoryComponent.h"
#include "Item/ItemDataRow.h"
#include "Attribute/AttributeTags.h"
#include "IngameUI/UIInventory.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"

bool UPopupItemAcquire::Initialize()
{
	if (Super::Initialize() == false)
		return false;

	if (BtnSkip)
	{
		BtnSkip->OnClicked.AddDynamic(this, &ThisClass::RemoveFromParent);
	}

	if (BtnTake)
	{
		BtnTake->OnClicked.AddDynamic(this, &ThisClass::TakeItem);
	}

	return true;
}

void UPopupItemAcquire::SetInventory(TWeakObjectPtr<UInventoryComponent> InInventory)
{
	Inventory = InInventory;

	if (UIInventory)
	{
		UIInventory->SetInventory(Inventory);
	}
}

void UPopupItemAcquire::SetOption()
{
	if (Inventory.IsValid() == false)
		return;

	const TArray<FItemSelectOption>& Options = Inventory->GetItemSelectOptionsInMisc(1);
	if (Options.IsEmpty() == true)
		return;
	Option = Options[0];

	FItemDataRow* OptionItemData = nullptr;
	const auto& Rows = MiscTable->GetRowMap();
	for (const auto& Row : Rows)
	{
		FItemDataRow* Data = (FItemDataRow*)Row.Value;
		if (Data != nullptr && Data->ItemId == Option.ItemId)
		{
			OptionItemData = Data;
			break;
		}
	}

	if (OptionItemData == nullptr)
		return;

	if (IsValid(TextRarity) == true)
	{
		const UEnum* RarityEnum = StaticEnum<EItemRarity>();
		if (RarityEnum)
		{
			FText Text = RarityEnum->GetDisplayNameTextByValue(static_cast<int64>(Option.Rarity));
			TextRarity->SetText(Text);
		}
		TextRarity->SetColorAndOpacity(FSlateColor(ColorTextRarity[Option.Rarity]));
	}

	if (IsValid(TextName) == true)
	{
		TextName->SetText(OptionItemData->ItemName);
	}

	if (IsValid(TextDesc) == true)
	{
		TextDesc->SetText(OptionItemData->ItemDesc);
	}

	if (IsValid(ImgIcon) == true)
	{
		LoadIcon(OptionItemData->ItemIcon);
	}
}

void UPopupItemAcquire::TakeItem()
{
	if (Inventory.IsValid() == false)
		return;

	Inventory->AddOrUpgradeItem(Option);
	RemoveFromParent();
}

void UPopupItemAcquire::LoadIcon(const TSoftObjectPtr<UTexture2D>& IconTexture)
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
