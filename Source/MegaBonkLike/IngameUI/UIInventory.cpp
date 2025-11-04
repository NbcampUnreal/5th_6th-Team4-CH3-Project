#include "IngameUI/UIInventory.h"
#include "Character/InventoryComponent.h"
#include "IngameUI/UIHorizontalItemList.h"
#include "Item/ItemEnums.h"

void UUIInventory::SetInventory(TWeakObjectPtr<UInventoryComponent> InInventory)
{
	Inventory = InInventory;

	UpdateItems();
}

void UUIInventory::UpdateItems()
{
	if (Inventory.IsValid() == false)
		return;

	if (IsValid(WeaponList) == true)
	{
		WeaponList->SetItems(Inventory->GetCachedItems(EItemType::Weapon));
	}

	if (IsValid(TomesList) == true)
	{
		TomesList->SetItems(Inventory->GetCachedItems(EItemType::Tomes));
	}

	if (IsValid(TomesList) == true)
	{
		MiscList->SetItems(Inventory->GetCachedItems(EItemType::Misc));
	}
}