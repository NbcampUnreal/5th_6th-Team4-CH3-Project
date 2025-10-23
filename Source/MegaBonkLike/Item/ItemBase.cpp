#include "Item/ItemBase.h"
#include "Item/ItemDataRow.h"
#include "Entity/AttributeComponent.h"

void UItemBase::SetData(const FItemDataRow* InData)
{
    Data = InData;
}

void UItemBase::Upgrade()
{
    ++Level;
}

float UItemBase::GetRarityMultiplier(EItemRarity InRarity)
{
	if (Data->ItemType != EItemType::Misc)
		return 1.0f;

	// 나중에 데이터 테이블로 바꾸는 게 나을 듯
	switch (InRarity)
	{
		case EItemRarity::Common: return 1.0f;
		case EItemRarity::Uncommon: return 1.2f;
		case EItemRarity::Rare: return 1.4f;
		case EItemRarity::Epic: return 1.6f;
		case EItemRarity::Legendary: return 2.0f;
		default: return 1.0f;
	}
}
