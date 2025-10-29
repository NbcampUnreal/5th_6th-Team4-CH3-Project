#pragma once

#include "CoreMinimal.h"
#include "Skill/SkillActionBase.h"
#include "SA_UnderFootAttack.generated.h"

class ADamageAreaActor;

UCLASS()
class MEGABONKLIKE_API USA_UnderFootAttack : public USkillActionBase
{
	GENERATED_BODY()
	
public:
	virtual void Activate(TWeakObjectPtr<AActor> InInstigator) override;

private:
	void SpawnUnderFootAttackActor();

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<ADamageAreaActor> SpawnActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName TargetTag;
};
