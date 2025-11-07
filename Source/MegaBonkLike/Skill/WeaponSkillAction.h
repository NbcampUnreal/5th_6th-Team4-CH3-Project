#pragma once

#include "CoreMinimal.h"
#include "Skill/SkillActionBase.h"
#include "WeaponSkillAction.generated.h"

UCLASS(Abstract)
class MEGABONKLIKE_API UWeaponSkillAction : public USkillActionBase
{
	GENERATED_BODY()

public:
	virtual float GetValue(const FGameplayTag& AttributeTag) const override;

	void SetOwnerWeapon(TWeakObjectPtr<UWeaponItem> InOwnerWeapon);

protected:
	struct FAttackData CreateAttackDataBase();

protected:
	UPROPERTY()
	TWeakObjectPtr<UWeaponItem> OwnerWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bKnockback = true;
};
