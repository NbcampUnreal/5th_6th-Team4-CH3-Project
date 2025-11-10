#pragma once

#include "CoreMinimal.h"
#include "SkillEffectSet.generated.h"

class USkillEffectBase;

USTRUCT(BlueprintType)
struct MEGABONKLIKE_API FSkillEffectSet
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<USkillEffectBase>> Effects;
	
	void SetOwner(TWeakObjectPtr<AActor> InOwner);

	void ActivateAll(const FVector& Location = FVector::ZeroVector, const FRotator& Rotation = FRotator::ZeroRotator, float InScale = 1.0f);
	void DeactivateAll();
};
