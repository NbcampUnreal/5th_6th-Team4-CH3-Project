#include "MBLChestObject.h"
#include "Gimmick/Spawn/MBLSpawnSubsystem.h"
#include "Character/MBLPlayerCharacter.h"
#include "Character/InventoryComponent.h"
#include "Item/ItemDataRow.h"
#include "Item/MiscItemWeightDataRow.h"

AMBLChestObject::AMBLChestObject() 
	: ItemTable(nullptr)
	, ItemWeight(nullptr)
{
}

void AMBLChestObject::OnObjectActivated(AActor* Activator)
{
	if (!IsValid(ItemTable)) return;

	static const FString ItemString(TEXT("ItemTable_OnObjectActivated_ItemCheck"));
	TArray<FItemDataRow*> ItemRows;
	ItemTable->GetAllRows(ItemString, ItemRows);

	if (ItemRows.IsEmpty()) return;

	if (!IsValid(ItemWeight)) return;

	static const FString WeightString(TEXT("ItemWeight_OnObjectActivated_WeightCheck"));
	TArray<FMiscItemWeightDataRow*> WeightRows;
	ItemWeight->GetAllRows(WeightString, WeightRows);

	if (WeightRows.IsEmpty()) return;

	float TotalChance = 0.0f;

	for (const FMiscItemWeightDataRow* Row : WeightRows)
	{
		if (Row)
		{
			TotalChance += Row->Weight;
		}
	}

	const float RandValue = FMath::FRandRange(0.0f, TotalChance);
	float AccumulateChance = 0.0f;

	for (FMiscItemWeightDataRow* Row : WeightRows)
	{
		AccumulateChance += Row->Weight;
		if (RandValue <= AccumulateChance)
		{
			if (AMBLPlayerCharacter* Player = Cast<AMBLPlayerCharacter>(Activator))
			{
				UInventoryComponent* Inventory = Player->FindComponentByClass<UInventoryComponent>();
				if (!Inventory) return;

				//Inventory->AddOrUpgradeItem()
			}
		}
	}
	
	

	// 기타 아이템 테이블 변수에 넣고
	// 테이블 2개
	// 순회할 때 레어도 체크하고
	// 기타 아이템 확률 테이블
	// 가중치의 합을 구하고
	// Activator->Cast()Player->FindComponentsByClass<UInventoryComponent>()
	// UInventoryComponent ㅁㄴㅇ->AddOrUpgradeItem(Row->ItemId)


}
