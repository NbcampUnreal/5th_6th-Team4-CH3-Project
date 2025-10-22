#include "Entity/InventoryComponent.h"
#include "Item/ItemDataRow.h"
#include "Entity/SkillComponent.h"
#include "Entity/AttributeComponent.h"
#include "Skill/Skill.h"
#include "Item/ItemBase.h"
#include "Kismet/KismetSystemLibrary.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	for (EItemType Type : TEnumRange<EItemType>())
	{
		CachedItemsByType.Add(Type, TArray<UItemBase*>());
	}
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInventoryComponent::AddOrUpgradeItem(int32 InItemId, EItemRarity InRarity)
{
	UItemBase* Item = FindItem(InItemId);
	if (IsValid(Item) == false)
	{
		AddItem(InItemId);
	}
	else
	{
		UpgradeItem(InItemId, InRarity);
	}
}

void UInventoryComponent::AddItem(int32 InItemId)
{
	const FItemDataRow* Data = GetItemData(InItemId);
	if (Data == nullptr)
		return;

	UItemBase* Item = FindItem(InItemId);
	if (Item != nullptr)
		return;

	Item = NewObject<UItemBase>();
	Item->InitializeItem(Data);
	if (Data->ItemType == EItemType::Weapon)
		AddWeaponSkill(Item);
	Items.Add(Item);
	CachedItemsByType[Data->ItemType].Add(Item);
}

void UInventoryComponent::UpgradeItem(int32 InItemId, EItemRarity InRarity)
{
	const FItemDataRow* Data = GetItemData(InItemId);
	if (Data == nullptr)
		return;

	UItemBase* Item = FindItem(InItemId);
	if (Item == nullptr)
		return;

	Item->Upgrade();	
	if (UAttributeComponent* AttributeComponent = GetOwner()->FindComponentByClass<UAttributeComponent>())
	{
		Item->AddAttributeModifiers(AttributeComponent, InRarity);
	}
}

void UInventoryComponent::RemoveItem(int32 InItemId)
{
	UItemBase* Item = FindItem(InItemId);
	if (Item == nullptr)
		return;

	if (Item->GetData()->ItemType == EItemType::Weapon)
		RemoveWeaponSkill(Item);

	UAttributeComponent* AttributeComponent = GetOwner()->FindComponentByClass<UAttributeComponent>();
	Item->RemoveAttributeModifiers(AttributeComponent);
	Items.Remove(Item);
	CachedItemsByType[Item->GetData()->ItemType].Remove(Item);
}

UItemBase* UInventoryComponent::FindItem(int32 InItemId)
{
	auto* Item = Items.FindByPredicate(
		[InItemId](const TObjectPtr<UItemBase>& Item)
		{
			return Item->GetData()->ItemId == InItemId;
		});
	return Item ? *Item : nullptr;
}

const FItemDataRow* UInventoryComponent::GetItemData(EItemType InItemType, int32 InItemId) const
{
	auto* DataTable = ItemDataTables.Find(InItemType);
	if (DataTable == nullptr)
		return nullptr;
	
	const auto& Rows = (*DataTable)->GetRowMap();
	for (auto& Row : Rows)
	{
		FItemDataRow* Data = (FItemDataRow*)Row.Value;
		if (Data->ItemId == InItemId)
		{
			return Data;
		}
	}
	return nullptr;
}

const FItemDataRow* UInventoryComponent::GetItemData(int32 InItemId) const
{
	for (EItemType Type : TEnumRange<EItemType>())
	{
		const FItemDataRow* Data = GetItemData(Type, InItemId);
		if (Data)
			return Data;
	}
	return nullptr;
}

void UInventoryComponent::AddWeaponSkill(UItemBase* Item)
{
	auto* Data = (FWeaponItemDataRow*)Item->GetData();
	if (Data == nullptr)
		return;

	if (USkillComponent* SkillComponent = GetOwner()->FindComponentByClass<USkillComponent>())
	{
		USkill* CDO = Data->SkillClass->GetDefaultObject<USkill>();
		USkill* NewSkill = DuplicateObject<USkill>(CDO, SkillComponent);
		int32 SkillId = SkillComponent->AddSkill(NewSkill);
		Item->SetSkillId(SkillId);
		NewSkill->SetOwnerWeapon(Item);
	}
}

void UInventoryComponent::RemoveWeaponSkill(UItemBase* Item)
{
	if (IsValid(Item) == false)
		return;

	if (USkillComponent* SkillComponent = GetOwner()->FindComponentByClass<USkillComponent>())
	{
		SkillComponent->RemoveSkill(Item->GetSkillId());
	}
}
