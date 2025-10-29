#pragma once

#include "CoreMinimal.h"
#include "Item/ItemBase.h"
#include "Attribute/AttributeSet.h"
#include "WeaponItem.generated.h"

UCLASS()
class MEGABONKLIKE_API UWeaponItem : public UItemBase
{
	GENERATED_BODY()

public:
	virtual void AddAttributeModifiers(UAttributeComponent*, EItemRarity InRarity = EItemRarity::Common) override;

	int32 GetSkillId() const { return SkillId; }
	void SetSkillId(int32 InSkillId);

	void AddAttributeChangedCallback(const FGameplayTag& Tag, TWeakObjectPtr<UObject> Instigator, const TFunction<void(const TWeakObjectPtr<UAttribute>)> NewCallback);
	void RemoveAttributeChangedCallback(const FGameplayTag& Tag, TWeakObjectPtr<UObject> Instigator);
	float GetAttributeValue(const FGameplayTag& AttributeTag) const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 SkillId;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FAttributeSet WeaponAttributeSet;
};
