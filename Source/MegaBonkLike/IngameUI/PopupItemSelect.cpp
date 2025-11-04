#include "IngameUI/PopupItemSelect.h"
#include "IngameUI/UIItemSelectOption.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/PanelWidget.h"
#include "Character/InventoryComponent.h"
#include "PopupItemAcquire.h"
#include "IngameUI/UIInventory.h"

bool UPopupItemSelect::Initialize()
{
	if (Super::Initialize() == false)
		return false;

	if (BtnBanish)
	{

	}

	if (BtnSkip)
	{
		BtnSkip->OnClicked.AddDynamic(this, &ThisClass::RemoveFromParent);
	}

	if (BtnRefresh)
	{
		BtnRefresh->OnClicked.AddDynamic(this, &ThisClass::RefreshOptions);
	}

	return true;
}

void UPopupItemSelect::SetInventory(TWeakObjectPtr<UInventoryComponent> InInventory)
{
	Inventory = InInventory;

	if (UIInventory)
	{
		UIInventory->SetInventory(Inventory);
	}
}

void UPopupItemSelect::SetOptions(int32 Count)
{
	if (Inventory.IsValid() == false)
		return;

	OptionCount = Count;

	OptionParent->ClearChildren();
	const TArray<FItemSelectOption>& Options = Inventory->GetItemSelectOptionsInWeaponAndTomes(Count);
	for (const auto& Option : Options)
	{
		UUIItemSelectOption* UIOption = CreateWidget<UUIItemSelectOption>(GetWorld(), UIOptionClass);
		if (UIOption)
		{
			OptionParent->AddChild(UIOption);
			UIOption->SetOption(Option);
			UIOption->OnItemSelected.AddDynamic(this, &ThisClass::SelectItem);
		}
	}
}

void UPopupItemSelect::SelectItem(const FItemSelectOption& Option)
{
	if (Inventory.IsValid() == false)
		return;
	
	Inventory->AddOrUpgradeItem(Option);
	RemoveFromParent();
}

void UPopupItemSelect::RefreshOptions()
{
	SetOptions(OptionCount);
}
