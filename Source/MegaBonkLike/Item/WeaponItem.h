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
	virtual void Init(AActor* InOwner, const FItemDataRow* InData) override;
	virtual void Upgrade(const FItemUpgradeContext& UpgradeContext) override;
	virtual void OnDestroy() override;

	int32 GetLevel() const { return Level; }

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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 Level = 1;
};
