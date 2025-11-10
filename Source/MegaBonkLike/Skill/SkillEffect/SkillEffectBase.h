#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SkillEffectBase.generated.h"

UCLASS(Abstract, Blueprintable, EditInlineNew, DefaultToInstanced)
class MEGABONKLIKE_API USkillEffectBase : public UObject
{
	GENERATED_BODY()
	
public:
	void SetOwner(TWeakObjectPtr<AActor> InOwner);

	virtual void Activate(const FVector& Location, const FRotator& Rotation, float InScale) {}
	virtual void Deactivate() {}

protected:
	UPROPERTY()
	TWeakObjectPtr<AActor> Owner;
};
