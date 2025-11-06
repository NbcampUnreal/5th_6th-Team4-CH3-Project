#include "IngameUI/UIItemSelectOption.h"
#include "Item/ItemDataRow.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Item/ItemEnums.h"
#include "UObject/EnumProperty.h"
#include "Attribute/AttributeTags.h"
#include "Components/Image.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"

bool UUIItemSelectOption::Initialize()
{
	if (Super::Initialize() == false)
		return false;

	if (ButtonOption)
	{
		ButtonOption->OnClicked.AddDynamic(this, &ThisClass::OnItemButtonClicked);
	}

	return true;
}

void UUIItemSelectOption::SetOption(const FItemSelectOption& InOption)
{
	ItemOption = InOption;

	FItemDataRow* OptionItemData = nullptr;
	const auto& Rows = ItemTables[InOption.ItemType]->GetRowMap();
	for (const auto& Row : Rows)
	{
		FItemDataRow* Data = (FItemDataRow*)Row.Value;
		if (Data != nullptr && Data->ItemId == InOption.ItemId)
		{
			OptionItemData = Data;
			break;
		}
	}

	if (OptionItemData == nullptr)
		return;

	if (IsValid(TextName) == true)
	{
		TextName->SetText(OptionItemData->ItemName);
	}

	if (IsValid(TextDesc) == true)
	{
		if (InOption.bIsNewItem && InOption.ItemType != EItemType::Tomes)
		{
			TextDesc->SetText(OptionItemData->ItemDesc);
		}
		else
		{
			FString String;
			for (const auto& Change : InOption.AttributeChanges)
			{
				String += FString::Printf(TEXT("%s: %.2f ➔ %.2f"), *GetTagName(Change.AttributeTag), Change.CurrentValue, Change.NewValue);
				String += TEXT("\n");
			}
			FString Result = String.TrimEnd();
			TextDesc->SetText(FText::FromString(Result));
		}
	}

	if (IsValid(TextRarity) == true)
	{
		if (InOption.bIsNewItem)
		{
			TextRarity->SetText(FText::FromString(TEXT("New")));
		}
		else
		{
			const UEnum* RarityEnum = StaticEnum<EItemRarity>();
			if (RarityEnum)
			{
				FText Text = RarityEnum->GetDisplayNameTextByValue(static_cast<int64>(InOption.Rarity));
				TextRarity->SetText(Text);
			}
			TextRarity->SetColorAndOpacity(FSlateColor(ColorTextRarity[InOption.Rarity]));
			ImgBackground->SetColorAndOpacity(ColorImgBackground[InOption.Rarity]);
		}
	}
	
	if (IsValid(TextLevel) == true)
	{
		FText Text = InOption.bIsNewItem == true ? 
			FText::FromString(TEXT("New")) : 
			FText::FromString(FString::Printf(TEXT("LVL %d"), InOption.Level));
		TextLevel->SetText(Text);
	}

	if (IsValid(ImgIcon) == true)
	{
		LoadIcon(OptionItemData->ItemIcon);
	}
}

void UUIItemSelectOption::LoadIcon(const TSoftObjectPtr<UTexture2D>& IconTexture)
{
	if (IconTexture.IsNull())
	{
		ImgIcon->SetVisibility(ESlateVisibility::Collapsed);
		ImgIcon->SetBrushFromTexture(nullptr);
		return;
	}

	ImgIcon->SetVisibility(ESlateVisibility::Visible);
	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
	Streamable.RequestAsyncLoad(IconTexture.ToSoftObjectPath(), [this, IconTexture]()
		{
			if (UTexture2D* LoadedIcon = IconTexture.Get())
			{
				ImgIcon->SetBrushFromTexture(LoadedIcon);
			}
		});
}

void UUIItemSelectOption::OnItemButtonClicked()
{
	OnItemSelected.Broadcast(ItemOption);
}
