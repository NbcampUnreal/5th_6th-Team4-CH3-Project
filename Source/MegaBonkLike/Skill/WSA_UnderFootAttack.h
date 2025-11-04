#pragma once

#include "CoreMinimal.h"
#include "Skill/WeaponSkillAction.h"
#include "WSA_UnderFootAttack.generated.h"

class ADamageAreaActor;

UCLASS()
class MEGABONKLIKE_API UWSA_UnderFootAttack : public UWeaponSkillAction
{
	GENERATED_BODY()
	
public:
	virtual void Activate(TWeakObjectPtr<AActor> InInstigator) override;

private:
	void SpawnUnderFootAttackActor();

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<ADamageAreaActor> SpawnActorClass;
};
