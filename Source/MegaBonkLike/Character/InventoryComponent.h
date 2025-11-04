#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

enum class EItemType : uint8;
class UItemBase;
struct FItemDataRow;
struct FItemSelectOption;
struct FItemUpgradeContext;

USTRUCT()
struct FCachedItems
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<TWeakObjectPtr<UItemBase>> Items;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnItemChanged);

UCLASS( ClassGroup=(Character), meta=(BlueprintSpawnableComponent) )
class MEGABONKLIKE_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

	virtual void BeginPlay() override;

	void AddOrUpgradeItem(const FItemSelectOption& SelectedOption);
	void AddItem(int32 InItemId);
	void UpgradeItem(const FItemUpgradeContext& Context);
	void RemoveItem(int32 InItemId);
	UItemBase* FindItem(int32 InItemId) const;

	TArray<TObjectPtr<UItemBase>> GetItems() const { return Items; }
	const TArray<TWeakObjectPtr<UItemBase>>& GetCachedItems(EItemType InItemType) const { return CachedItemsByType[InItemType].Items; }

	bool CanAddItem(EItemType InType) const;

private:
	const FItemDataRow* GetItemData(EItemType InItemType, int32 InItemId) const;
	const FItemDataRow* GetItemData(int32 InItemId) const;

public:
	FOnItemChanged OnItemChanged;

#pragma region Upgrade
public:
	TArray<FItemSelectOption> GetItemSelectOptionsInWeaponAndTomes(int Count) const;
	TArray<FItemSelectOption> GetItemSelectOptionsInMisc(int Count) const;

private:
	const struct FItemRarityDataRow* GetRandomRarity() const;
#pragma endregion

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EItemType, TObjectPtr<UDataTable>> ItemDataTables;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UDataTable> MiscItemWeightTable;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UDataTable> ItemRarityTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<UItemBase>> Items;

	// UI에서 보여주기용 타입별 분류 캐싱
	TMap<EItemType, FCachedItems> CachedItemsByType;

	UPROPERTY(EditAnywhere)
	int32 MaxWeaponCount = 4;
	UPROPERTY(EditAnywhere)
	int32 MaxTomesCount = 4;
};
