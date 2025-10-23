#pragma once

#include "CoreMinimal.h"
#include "Item/ItemBase.h"
#include "MiscItem.generated.h"

UCLASS()
class MEGABONKLIKE_API UMiscItem : public UItemBase
{
	GENERATED_BODY()

public:
	virtual void AddAttributeModifiers(UAttributeComponent* AttributeComponent, EItemRarity) override;
	virtual void RemoveAttributeModifiers(UAttributeComponent* AttributeComponent) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TMap<FGameplayTag, int32> ModifierIds;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TMap<FGameplayTag, FAttributeModifier> Modifiers;
};
