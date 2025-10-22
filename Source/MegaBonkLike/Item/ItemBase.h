#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Attribute/AttributeModifier.h"
#include "GameplayTagContainer.h"
#include "Item/ItemEnums.h"
#include "ItemBase.generated.h"

class UAttributeComponent;
struct FItemDataRow;

UCLASS()
class MEGABONKLIKE_API UItemBase : public UObject
{
	GENERATED_BODY()

public:
	virtual void InitializeItem(const FItemDataRow* InData);
	void Upgrade();

	void AddAttributeModifiers(UAttributeComponent* AttributeComponent, EItemRarity InRarity = EItemRarity::Common);
	void RemoveAttributeModifiers(UAttributeComponent* AttributeComponent);

	int32 GetSkillId() const { return SkillId; }
	void SetSkillId(int32 InSkillId);

	const FAttributeModifier* GetModifier(const FGameplayTag& AttributeTag) const;

	const FItemDataRow* GetData() const { return Data; }

protected:
	float GetRarityMultiplier(EItemRarity InRarity);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Level = 1;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 SkillId;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TMap<FGameplayTag, int32> ModifierIds;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TMap<FGameplayTag, FAttributeModifier> Modifiers;

	const FItemDataRow* Data = nullptr;
};
