#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Attribute/AttributeModifier.h"
#include "GameplayTagContainer.h"
#include "Item/ItemEnums.h"
#include "ItemBase.generated.h"

class UAttributeComponent;
struct FItemDataRow;

UCLASS(Abstract)
class MEGABONKLIKE_API UItemBase : public UObject
{
	GENERATED_BODY()

public:
	void SetData(const FItemDataRow* InData);
	void Upgrade();

	virtual void AddAttributeModifiers(UAttributeComponent* AttributeComponent, EItemRarity InRarity = EItemRarity::Common) {}
	virtual void RemoveAttributeModifiers(UAttributeComponent* AttributeComponent) {}

	int32 GetLevel() const { return Level; }
	const FItemDataRow* GetData() const { return Data; }

protected:
	float GetRarityMultiplier(EItemRarity InRarity);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Level = 1;

	const FItemDataRow* Data = nullptr;
};
