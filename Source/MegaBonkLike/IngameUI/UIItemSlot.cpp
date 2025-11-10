#include "IngameUI/UIItemSlot.h"
#include "Components/TextBlock.h"
#include "Item/WeaponItem.h"
#include "Item/TomesItem.h"
#include "Item/MiscItem.h"
#include "Item/ItemDataRow.h"
#include "Components/Image.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "IngameUI/UIItemSlotInfo.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void UUIItemSlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	if (Item.IsValid() == false)
		return;

	if (IsValid(UIInfo) == false)
	{
		if (IsValid(UIInfoClass) == true)
		{
			UIInfo = CreateWidget<UUIItemSlotInfo>(GetWorld(), UIInfoClass);
			UIInfo->AddToViewport(10);
		}
	}

	if (IsValid(UIInfo) == true)
	{
		UIInfo->SetVisibility(ESlateVisibility::HitTestInvisible);
		UIInfo->SetInfo(Item);
		bInfoActive = true;
	}
}

void UUIItemSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	if (IsValid(UIInfo) == true)
	{
		UIInfo->SetVisibility(ESlateVisibility::Hidden);
	}
	bInfoActive = false;
}

void UUIItemSlot::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bInfoActive)
	{
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (IsValid(PlayerController) == false)
			return;

		FVector2D InfoPos;
		UWidgetLayoutLibrary::GetMousePositionScaledByDPI(PlayerController, InfoPos.X, InfoPos.Y);

		FVector2D ViewportSize = UWidgetLayoutLibrary::GetViewportSize(GetWorld());
		FVector2D InfoSize = UIInfo->GetDesiredSize();

		InfoPos.X = FMath::Clamp(InfoPos.X, 0.0f, ViewportSize.X - InfoSize.X);
		InfoPos.Y = FMath::Clamp(InfoPos.Y - InfoSize.Y, 0.0f, ViewportSize.Y - InfoSize.Y);

		UIInfo->SetPositionInViewport(InfoPos, false);
	}
}

void UUIItemSlot::SetItem(TWeakObjectPtr<UItemBase> InItem)
{
	Item = InItem;
	if (Item.IsValid() == false)
		return;

	const FItemDataRow* Data = Item->GetData();
	if (Data == nullptr)
		return;

	if (IsValid(UIInfo) == true)
	{
		UIInfo->SetInfo(Item);
	}

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
