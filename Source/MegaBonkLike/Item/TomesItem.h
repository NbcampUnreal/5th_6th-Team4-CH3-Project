#pragma once

#include "CoreMinimal.h"
#include "Item/ItemBase.h"
#include "TomesItem.generated.h"

UCLASS()
class MEGABONKLIKE_API UTomesItem : public UItemBase
{
	GENERATED_BODY()

public:
	virtual void AddAttributeModifiers(UAttributeComponent* AttributeComponent, EItemRarity InRarity) override;
	virtual void RemoveAttributeModifiers(UAttributeComponent* AttributeComponent) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TMap<FGameplayTag, int32> ModifierIds;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TMap<FGameplayTag, FAttributeModifier> Modifiers;
};
