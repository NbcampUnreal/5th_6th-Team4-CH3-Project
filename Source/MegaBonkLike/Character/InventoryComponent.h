#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Item/ItemEnums.h"
#include "InventoryComponent.generated.h"

class UItemBase;
struct FItemDataRow;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnItemChanged);

UCLASS( ClassGroup=(Character), meta=(BlueprintSpawnableComponent) )
class MEGABONKLIKE_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

	virtual void BeginPlay() override;

	void AddOrUpgradeItem(int32 InItemId, EItemRarity InRarity = EItemRarity::Common);
	void AddItem(int32 InItemId);
	void UpgradeItem(int32 InItemId, EItemRarity InRarity = EItemRarity::Common);
	void RemoveItem(int32 InItemId);
	UItemBase* FindItem(int32 InItemId);

	TArray<TObjectPtr<UItemBase>> GetItems() const { return Items; }
	const TArray<UItemBase*>& GetCachedItems(EItemType InItemType) { return CachedItemsByType[InItemType]; }

private:
	const FItemDataRow* GetItemData(EItemType InItemType, int32 InItemId) const;
	const FItemDataRow* GetItemData(int32 InItemId) const;
	void AddWeaponSkill(UItemBase* Item);
	void RemoveWeaponSkill(UItemBase* Item);

public:
	FOnItemChanged OnItemChanged;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EItemType, TObjectPtr<UDataTable>> ItemDataTables;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<UItemBase>> Items;

	// UI에서 보여주기용 타입별 분류 캐싱
	TMap<EItemType, TArray<UItemBase*>> CachedItemsByType;
};
