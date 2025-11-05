#include "IngameUI/UIItemSelectOption.h"
#include "Item/ItemDataRow.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Item/ItemEnums.h"
#include "UObject/EnumProperty.h"
#include "Attribute/AttributeTags.h"

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
		if (InOption.bIsNewItem)
		{
			TextDesc->SetText(OptionItemData->ItemDesc);
		}
		else
		{
			FString String;
			for (const auto& Change : InOption.AttributeChanges)
			{
				String += FString::Printf(TEXT("Gain +%.2f  "), Change.DeltaValue);
				String += GetTagName(Change.AttributeTag);
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
		}
	}
	
	if (IsValid(TextLevel) == true)
	{
		FText Text = InOption.bIsNewItem == true ? 
			FText::FromString(TEXT("New")) : 
			FText::FromString(FString::Printf(TEXT("LVL %d"), InOption.Level));
		TextLevel->SetText(Text);
	}
}

void UUIItemSelectOption::OnItemButtonClicked()
{
	OnItemSelected.Broadcast(ItemOption);
}
