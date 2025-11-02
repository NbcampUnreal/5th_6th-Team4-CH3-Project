#pragma once

#include "CoreMinimal.h"
#include "Skill/WeaponSkillAction.h"
#include "Skill/SkillEffect/SkillEffectSet.h"
#include "WSA_AttachToPlayer.generated.h"

class ADamageAreaActor;

UCLASS()
class MEGABONKLIKE_API UWSA_AttachToPlayer : public UWeaponSkillAction
{
	GENERATED_BODY()

public:
	virtual void Activate(TWeakObjectPtr<AActor> InInstigator) override;
	virtual void Deactivate() override;

private:
	void SetDamage();
	void SetSize();

	void CheckHit();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ADamageAreaActor> AttachedActorClass;
	UPROPERTY()
	TObjectPtr<ADamageAreaActor> AttachedActor;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector LocationOffset;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FRotator RotationOffset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FSkillEffectSet CheckHitEffects;
};
