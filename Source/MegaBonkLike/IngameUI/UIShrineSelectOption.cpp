#include "IngameUI/UIShrineSelectOption.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Item/ItemEnums.h"
#include "UObject/EnumProperty.h"
#include "Attribute/AttributeTags.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "Blueprint/WidgetTree.h"

const FName UUIShrineSelectOption::BorderNames[4] = { TEXT("BorderTop"), TEXT("BorderBottom"), TEXT("BorderLeft"), TEXT("BorderRight") };

bool UUIShrineSelectOption::Initialize()
{
	if (Super::Initialize() == false)
		return false;

	if (ButtonOption)
	{
		ButtonOption->OnClicked.AddDynamic(this, &ThisClass::OnButtonClicked);
	}

	for (const FName& BorderName : BorderNames)
	{
		if (UBorder* Border = WidgetTree->FindWidget<UBorder>(BorderName))
		{
			Borders.Add(Border);
		}
	}

	return true;
}

void UUIShrineSelectOption::SetOption(const FShrineOption& InOption)
{
	Option = InOption;

	if (IsValid(ImgBackground) == true)
	{
		ImgBackground->SetColorAndOpacity(ColorImgBackground[InOption.Rarity]);
	}

	if (IsValid(TextRarity) == true)
	{
		const UEnum* RarityEnum = StaticEnum<EItemRarity>();
		if (RarityEnum)
		{
			FText Text = RarityEnum->GetDisplayNameTextByValue(static_cast<int64>(Option.Rarity));
			TextRarity->SetText(Text);
		}
	}

	for (const auto& Border : Borders)
	{
		if (IsValid(Border) == true)
		{
			Border->SetBrushColor(ColorBorder[Option.Rarity]);
		}
	}

	if (IsValid(TextOptionName) == true)
	{
		TextOptionName->SetText(FText::FromString(GetTagName(Option.Tag)));
	}

	if (IsValid(TextOptionValue) == true)
	{
		TextOptionValue->SetText(FText::FromString(FString::Printf(TEXT("%.2f"), Option.Modifier.Value)));
	}
}

void UUIShrineSelectOption::OnButtonClicked()
{
	OnSelected.Broadcast(Option);
}
