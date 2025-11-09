#include "Character/InventoryComponent.h"
#include "Item/ItemDataRow.h"
#include "Character/SkillComponent.h"
#include "Character/AttributeComponent.h"
#include "Skill/Skill.h"
#include "Item/WeaponItem.h"
#include "Item/TomesItem.h"
#include "Item/MiscItem.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Algo/RandomShuffle.h"
#include "Item/ItemRarityDataRow.h"
#include "Item/ItemSelectOption.h"
#include "Item/ItemUpgradeContext.h"
#include "Item/MiscItemWeightDataRow.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	for (EItemType Type : TEnumRange<EItemType>())
	{
		CachedItemsByType.Add(Type, FCachedItems());
	}
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInventoryComponent::AddOrUpgradeItem(const FItemSelectOption& SelectedOption)
{
	if (SelectedOption.bIsNewItem == true)
	{
		AddItem(SelectedOption.ItemId);
	}
	else
	{
		FItemUpgradeContext UpgradeContext;
		UpgradeContext.ItemId = SelectedOption.ItemId;
		for (const auto& Changes : SelectedOption.AttributeChanges)
		{
			UpgradeContext.AttributeChanges.Add(Changes.AttributeTag, FAttributeModifier(Changes.ModifierType, Changes.DeltaValue));
		}
		UpgradeItem(UpgradeContext);
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

	switch (Data->ItemType)
	{
		case EItemType::Weapon: Item = NewObject<UWeaponItem>(); break;
		case EItemType::Tomes: Item = NewObject<UTomesItem>(); break;
		case EItemType::Misc: Item = NewObject<UMiscItem>(); break;
		default: return;
	}
	Item->Init(GetOwner(), Data);
	Items.Add(Item);
	CachedItemsByType[Data->ItemType].Items.Add(Item);

	OnItemChanged.Broadcast();
}

void UInventoryComponent::UpgradeItem(const FItemUpgradeContext& Context)
{
	UItemBase* Item = FindItem(Context.ItemId);
	if (IsValid(Item) == false)
		return;

	Item->Upgrade(Context);

	OnItemChanged.Broadcast();
}

void UInventoryComponent::RemoveItem(int32 InItemId)
{
	UItemBase* Item = FindItem(InItemId);
	if (Item == nullptr)
		return;

	Item->OnDestroy();

	Items.Remove(Item);
	CachedItemsByType[Item->GetData()->ItemType].Items.Remove(Item);

	OnItemChanged.Broadcast();
}

UItemBase* UInventoryComponent::FindItem(int32 InItemId) const
{
	auto* Item = Items.FindByPredicate(
		[InItemId](const TObjectPtr<UItemBase>& Item)
		{
			return Item->GetData()->ItemId == InItemId;
		});
	return Item ? *Item : nullptr;
}

bool UInventoryComponent::CanAddItem(EItemType InType) const
{
	switch (InType)
	{
		case EItemType::Weapon: 
			return CachedItemsByType[EItemType::Weapon].Items.Num() < MaxWeaponCount;
		case EItemType::Tomes:
			return CachedItemsByType[EItemType::Tomes].Items.Num() < MaxTomesCount;
		default:
			return true;
	}
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

TArray<FItemSelectOption> UInventoryComponent::GetItemSelectOptionsInWeaponAndTomes(int Count) const
{
	TArray<FItemSelectOption> Results;

	TArray<const FItemDataRow*> TargetDatas;
	auto GetTargetDatas =
		[&](EItemType Type)
		{
			if (CanAddItem(Type) == true)
			{
				const auto& Rows = ItemDataTables[Type]->GetRowMap();
				for (const auto& Row : Rows)
				{
					FItemDataRow* Data = (FItemDataRow*)Row.Value;
					TargetDatas.Add(Data);
				}
			}
			else
			{
				const auto& CurrItems = GetCachedItems(Type);
				for (const auto& Item : CurrItems)
				{
					TargetDatas.Add(Item->GetData());
				}
			}
		};

	GetTargetDatas(EItemType::Weapon);
	GetTargetDatas(EItemType::Tomes);

	Algo::RandomShuffle(TargetDatas);

	for (int i = 0; i < Count && i < TargetDatas.Num(); ++i)
	{
		const FItemRarityDataRow* Rarity = GetRandomRarity();
		if (Rarity == nullptr)
			continue;

		FItemSelectOption Result;
		FItemUpgradeContext UpgradeContext;
		Result.ItemId = TargetDatas[i]->ItemId;
		Result.ItemType = TargetDatas[i]->ItemType;
		Result.Rarity = Rarity->ItemRarity;
		UItemBase* ExistItem = FindItem(Result.ItemId);
		Result.bIsNewItem = ExistItem == nullptr;
		if (TargetDatas[i]->ItemType == EItemType::Weapon)
		{
			const UWeaponItem* ExistWeapon = Cast<UWeaponItem>(ExistItem);
			Result.Level = ExistWeapon == nullptr ? 0 : ExistWeapon->GetLevel();

			FWeaponItemDataRow* WeaponData = (FWeaponItemDataRow*)TargetDatas[i];
			int EntryCount = WeaponData->WeaponAttributeEntry.Num();
			TArray<int32> IdxArray;
			IdxArray.SetNum(EntryCount);
			for (int j = 0; j < EntryCount; ++j)
			{
				IdxArray[j] = j;
			}
			Algo::RandomShuffle(IdxArray);
			int UpgradeEntryCount = FMath::Min(EntryCount, (Result.Rarity > EItemRarity::Common ? 2 : FMath::RandRange(1, 2)));
			for (int j = 0; j < UpgradeEntryCount; ++j)
			{
				FAttributeComparison NewComparison;
				const auto& UpgradeEntry = WeaponData->WeaponAttributeEntry[IdxArray[j]];
				NewComparison.AttributeTag = UpgradeEntry.AttributeTag;
				NewComparison.ModifierType = UpgradeEntry.UpgradeModifier.Type;
				NewComparison.CurrentValue = ExistWeapon == nullptr ? 0.0f : ExistWeapon->GetAttributeValue(UpgradeEntry.AttributeTag);
				NewComparison.DeltaValue = ExistWeapon == nullptr ? UpgradeEntry.BaseValue : (UpgradeEntry.UpgradeModifier.Value * Rarity->Multiplier);
				NewComparison.NewValue = NewComparison.CurrentValue + NewComparison.DeltaValue;
				Result.AttributeChanges.Add(NewComparison);
			}
		}
		else if (TargetDatas[i]->ItemType == EItemType::Tomes)
		{
			FTomesItemDataRow* TomesData = (FTomesItemDataRow*)TargetDatas[i];
			if (TomesData->AttributeModifiers.IsEmpty() == true)
				continue;

			const UTomesItem* ExistTomes = Cast<UTomesItem>(ExistItem);
			Result.Level = ExistTomes == nullptr ? 0 : ExistTomes->GetLevel();

			TArray<FGameplayTag> Keys;
			TomesData->AttributeModifiers.GetKeys(Keys);
			int32 RandomKeyIdx = FMath::RandRange(0, TomesData->AttributeModifiers.Num() - 1);
			FGameplayTag Tag = Keys[RandomKeyIdx];
			const auto& UpgradeModifier = TomesData->AttributeModifiers[Tag];
			FAttributeComparison NewComparison;
			NewComparison.AttributeTag = Tag;
			NewComparison.ModifierType = UpgradeModifier.Type;
			NewComparison.CurrentValue = ExistTomes == nullptr ? 0.0f : ExistTomes->GetModifierValue(Tag);
			NewComparison.DeltaValue = UpgradeModifier.Value * (ExistTomes == nullptr ? 1.0f : Rarity->Multiplier);
			NewComparison.NewValue = NewComparison.CurrentValue + NewComparison.DeltaValue;
			Result.AttributeChanges.Add(NewComparison);
		}

		Results.Add(Result);
	}

	return Results;
}

TArray<FItemSelectOption> UInventoryComponent::GetItemSelectOptionsInMisc(int Count) const
{
	TArray<FItemSelectOption> Results;

	const auto& MiscWeightRows = MiscItemWeightTable->GetRowMap();
	TMap<EItemRarity, float> MiscWeightMap;
	for (const auto& Row : MiscWeightRows)
	{
		FMiscItemWeightDataRow* Data = (FMiscItemWeightDataRow*)Row.Value;
		MiscWeightMap.Add(Data->ItemRarity, Data->Weight);
	}

	const auto& MiscTable = ItemDataTables[EItemType::Misc]->GetRowMap();
	float TotalMiscWeight = 0.0f;
	TMap<int32, EItemRarity> ItemRarities;
	for (const auto& Row : MiscTable)
	{
		FMiscItemDataRow* Data = (FMiscItemDataRow*)Row.Value;
		ItemRarities.Add(Data->ItemId, Data->ItemRarity);
		float ItemWeight = MiscWeightMap[Data->ItemRarity];
		TotalMiscWeight += ItemWeight;
	}

	int32 MaxItemCount = FMath::Min(Count, ItemRarities.Num());
	for (int i = 0; i < MaxItemCount; ++i)
	{
		float RandomWeight = FMath::RandRange(0.0f, TotalMiscWeight);
		int32 SelectedId = ItemRarities.begin()->Key;
		for (const auto& [ItemId, ItemRarity] : ItemRarities)
		{
			RandomWeight -= MiscWeightMap[ItemRarity];
			if (RandomWeight <= 0.0f)
			{
				SelectedId = ItemId;
				break;
			}
		}

		FItemSelectOption Result;
		Result.ItemId = SelectedId;
		Result.ItemType = EItemType::Misc;
		Result.Rarity = ItemRarities[SelectedId];
		Result.bIsNewItem = FindItem(SelectedId) == nullptr;
		// FItemSelectOption의 나머지 요소 안 씀.
		Results.Add(Result);

		TotalMiscWeight -= MiscWeightMap[ItemRarities[SelectedId]];
		ItemRarities.Remove(SelectedId);
	}

	return Results;
}

const FItemRarityDataRow* UInventoryComponent::GetRandomRarity() const
{
	const auto& RarityRows = ItemRarityTable->GetRowMap();
	TArray<FItemRarityDataRow*> RarityArray;
	RarityArray.Reserve(RarityRows.Num());
	float TotalRarityAppearRate = 0.0f;
	for (const auto& Row : RarityRows)
	{
		FItemRarityDataRow* Data = (FItemRarityDataRow*)Row.Value;
		TotalRarityAppearRate += Data->AppearanceRate;
		RarityArray.Add(Data);
	}

	float RandomRate = FMath::RandRange(0.0f, TotalRarityAppearRate);
	for (auto* Rarity : RarityArray)
	{
		RandomRate -= Rarity->AppearanceRate;
		if (RandomRate <= 0.0f)
		{
			return Rarity;
		}
	}
	return RarityArray.IsEmpty() ? nullptr : *RarityArray.begin();
}
