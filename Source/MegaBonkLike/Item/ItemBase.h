#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Attribute/AttributeModifier.h"
#include "Item/ItemEnums.h"
#include "Item/ItemUpgradeContext.h"
#include "ItemBase.generated.h"

class UAttributeComponent;
struct FItemDataRow;

UCLASS(Abstract)
class MEGABONKLIKE_API UItemBase : public UObject
{
	GENERATED_BODY()

public:
	virtual void Init(AActor* InOwner, const FItemDataRow* InData);
	virtual void Upgrade(const FItemUpgradeContext& UpgradeContext) {}
	virtual void OnDestroy() {}

	const FItemDataRow* GetData() const { return Data; }

protected:
	class UAttributeComponent* GetOwnerAttributeComponent();

protected:
	TWeakObjectPtr<AActor> Owner;
	const FItemDataRow* Data = nullptr;
};
