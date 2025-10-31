#pragma once

#include "CoreMinimal.h"
#include "Skill/SkillEffect/SkillEffectBase.h"
#include "SE_Niagara.generated.h"

class UNiagaraSystem;

UCLASS()
class MEGABONKLIKE_API USE_Niagara : public USkillEffectBase
{
	GENERATED_BODY()

public:
	virtual void Activate(const FVector& Location, const FRotator& Rotation) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UNiagaraSystem> NiagaraSystem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bAttachToOwner;
};
