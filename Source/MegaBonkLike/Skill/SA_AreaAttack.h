#pragma once

#include "CoreMinimal.h"
#include "Skill/SkillActionBase.h"
#include "SA_AreaAttack.generated.h"

UCLASS(BlueprintType)
class MEGABONKLIKE_API USA_AreaAttack : public USkillActionBase
{
	GENERATED_BODY()

public:
	virtual void Activate(TWeakObjectPtr<AActor> InInstigator) override;

private:
	void CheckHit();

protected:
};
