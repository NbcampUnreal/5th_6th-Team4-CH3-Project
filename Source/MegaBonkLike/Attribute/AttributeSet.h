#pragma once

#include "CoreMinimal.h"
#include "Attribute/Attribute.h"
#include "AttributeSet.generated.h"

USTRUCT(BlueprintType)
struct MEGABONKLIKE_API FAttributeSet
{
	GENERATED_BODY()

    UPROPERTY(VisibleAnywhere)
    TMap<FGameplayTag, FAttribute> Attributes;

    void AddAttribute(const FGameplayTag& Tag, float InBaseValue);
    void AddAttribute(const FAttribute& Attribute);

    void AddChangedCallback(const FGameplayTag& Tag, const TFunction<void(const FAttribute&)> NewCallback);

    int32 AddModifier(const FGameplayTag& Tag, const FAttributeModifier& Modifier);
    void ChangeModifier(const FGameplayTag& Tag, int32 InModifierId, const FAttributeModifier& NewModifier);
    void RemoveModifier(const FGameplayTag& Tag, int32 ModifierId);

    bool HasAttribute(const FGameplayTag& Tag) const;
    float GetAttributeValue(const FGameplayTag& Tag) const;
};
