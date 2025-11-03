#pragma once

#include "CoreMinimal.h"
#include "Skill/SkillEffect/SkillEffectBase.h"
#include "SE_SoundEffect.generated.h"

UCLASS()
class MEGABONKLIKE_API USE_SoundEffect : public USkillEffectBase
{
	GENERATED_BODY()

public:
	virtual void Activate(const FVector& Location, const FRotator& Rotation) override;
	virtual void Deactivate() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USoundBase> Sound;
	UPROPERTY()
	TObjectPtr<UAudioComponent> Audio;
};
