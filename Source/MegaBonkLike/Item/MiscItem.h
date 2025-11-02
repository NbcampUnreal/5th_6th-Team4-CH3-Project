#pragma once

#include "CoreMinimal.h"
#include "Item/ItemBase.h"
#include "MiscItem.generated.h"

UCLASS()
class MEGABONKLIKE_API UMiscItem : public UItemBase
{
	GENERATED_BODY()

public:
	virtual void Init(AActor* InOwner, const FItemDataRow* InData) override;
	virtual void Upgrade(const FItemUpgradeContext&) override;
	virtual void OnDestroy() override;

	void AddSkill();

	int32 GetStackCount() const { return SkillIds.Num(); }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<int32> SkillIds;
};
