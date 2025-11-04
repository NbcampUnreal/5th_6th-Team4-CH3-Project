#pragma once

#include "CoreMinimal.h"
#include "Skill/WeaponSkillAction.h"
#include "WSA_AreaAttack.generated.h"

UCLASS(BlueprintType)
class MEGABONKLIKE_API UWSA_AreaAttack : public UWeaponSkillAction
{
	GENERATED_BODY()

public:
	virtual void Activate(TWeakObjectPtr<AActor> InInstigator) override;

private:
	void CheckHit();

protected:
};
