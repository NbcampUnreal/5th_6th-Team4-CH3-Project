#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameplayTagContainer.h"
#include "Attribute/AttributeModifier.h"
#include "BuffObjectDataManager.generated.h"

struct FShrineOption;

UCLASS()
class MEGABONKLIKE_API UBuffObjectDataManager : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	void ApplyShrineBuff(const FShrineOption& SelectedOption);

protected:
	UPROPERTY()
	TMap<FGameplayTag, int32> ModifierIds;
	UPROPERTY()
	TMap<FGameplayTag, FAttributeModifier> Modifiers;
};
