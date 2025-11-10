#pragma once

#include "CoreMinimal.h"
#include "Skill/WeaponSkillAction.h"
#include "Skill/SkillEffect/SkillEffectSet.h"
#include "WSA_UnderFootAttack.generated.h"

class ADamageAreaActor;

UCLASS()
class MEGABONKLIKE_API UWSA_UnderFootAttack : public UWeaponSkillAction
{
	GENERATED_BODY()
	
public:
	virtual void Activate(TWeakObjectPtr<AActor> InInstigator) override;
	virtual void Deactivate() override;

private:
	void SpawnUnderFootAttackActor();

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<ADamageAreaActor> SpawnActorClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FSkillEffectSet SpawnEffects;
};
