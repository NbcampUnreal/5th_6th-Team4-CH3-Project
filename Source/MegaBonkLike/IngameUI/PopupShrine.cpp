#include "IngameUI/PopupShrine.h"
#include "Character/InventoryComponent.h"
#include "IngameUI/UIInventory.h"
#include "Components/Button.h"
#include "IngameUI/UIShrineSelectOption.h"

bool UPopupShrine::Initialize()
{
	if (Super::Initialize() == false)
		return false;

	if (BtnIgnoreOffers)
	{
		BtnIgnoreOffers->OnClicked.AddDynamic(this, &ThisClass::RemoveFromParent);
	}

	return true;
}

void UPopupShrine::SetInventory(TWeakObjectPtr<UInventoryComponent> InInventory)
{
	Inventory = InInventory;

	if (UIInventory)
	{
		UIInventory->SetInventory(Inventory);
	}
}

void UPopupShrine::SetOptions(const TArray<FShrineOption>& Options)
{
	if (IsValid(OptionParent) == false)
		return;

	OptionParent->ClearChildren();

	int32 SlotCount = Options.Num();
	for (int32 i = 0; i < SlotCount; ++i)
	{
		UUIShrineSelectOption* NewOptionUI = CreateWidget<UUIShrineSelectOption>(GetWorld(), UIOptionClass);
		if (NewOptionUI)
		{
			NewOptionUI->SetOption(Options[i]);
			NewOptionUI->OnSelected.AddDynamic(this, &ThisClass::SelectOption);
			OptionParent->AddChild(NewOptionUI);
		}
	}
}

void UPopupShrine::SelectOption(const FShrineOption& SelectedOption)
{
	OnSelectedShrine.Broadcast(SelectedOption);
	RemoveFromParent();
}
