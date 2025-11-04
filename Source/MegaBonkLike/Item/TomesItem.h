#pragma once

#include "CoreMinimal.h"
#include "Item/ItemBase.h"
#include "GameplayTagContainer.h"
#include "TomesItem.generated.h"

UCLASS()
class MEGABONKLIKE_API UTomesItem : public UItemBase
{
	GENERATED_BODY()

public:
	virtual void Init(AActor* InOwner, const FItemDataRow* InData) override;
	virtual void Upgrade(const FItemUpgradeContext& UpgradeContext) override;
	virtual void OnDestroy() override;

	int32 GetLevel() const;

	void AddAttributeModifiers(const TMap<FGameplayTag, FAttributeModifier>& InModifiers);
	void RemoveAttributeModifiers();
	float GetModifierValue(const FGameplayTag& AttributeTag) const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TMap<FGameplayTag, int32> ModifierIds;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TMap<FGameplayTag, FAttributeModifier> Modifiers;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 Level = 1;
};
