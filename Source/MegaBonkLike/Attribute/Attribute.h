#pragma once

#include "CoreMinimal.h"
#include "Attribute/AttributeModifier.h"
#include "GameplayTagContainer.h"
#include "Attribute.generated.h"

USTRUCT(BlueprintType)
struct MEGABONKLIKE_API FAttribute
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FGameplayTag AttributeTag;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BaseValue;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<int32, FAttributeModifier> Modifiers;

	TArray<TFunction<void(const FAttribute&)>> OnChangedCallbacks;

	int32 NextId = 0;

	int32 AddModifier(const FAttributeModifier& InModifier);
	int32 AddModifier(EAttributeModifierType InType, float InValue);
	bool ChangeModifier(int32 InModifierId, const FAttributeModifier& NewModifier);
	void RemoveModifier(int32 InId);
	const FAttributeModifier* GetModifier(int32 InId) const;
	float GetValue(const TArray<FAttributeModifier>& InAdditiveModifiers = TArray<FAttributeModifier>()) const;

	void AddChangedCallback(TFunction<void(const FAttribute&)> NewCallback);
	void AddChangedCallback(TFunction<void()> NewCallBack);
	void BroadCastChanged();
};
