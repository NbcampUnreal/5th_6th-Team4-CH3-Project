#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Attribute/AttributeModifier.h"
#include "GameplayTagContainer.h"
#include "Attribute.generated.h"

UCLASS(BlueprintType)
class MEGABONKLIKE_API UAttribute : public UObject
{
	GENERATED_BODY()

public:
	void Init(const FGameplayTag& Tag, float InBaseValue);

	int32 AddModifier(const FAttributeModifier& InModifier);
	int32 AddModifier(EAttributeModifierType InType, float InValue);
	bool ChangeModifier(int32 InModifierId, const FAttributeModifier& NewModifier);
	void RemoveModifier(int32 InId);
	const FAttributeModifier* GetModifier(int32 InId) const;
	float GetValue() const;

	void RecalculateFinalValue();

	void AddChangedCallback(TWeakObjectPtr<UObject> Instigator, TFunction<void(const TWeakObjectPtr<UAttribute>)> NewCallback);
	void AddChangedCallback(TWeakObjectPtr<UObject> Instigator, TFunction<void()> NewCallBack);
	void RemoveChangedCallback(TWeakObjectPtr<UObject> Instigator);
	void BroadCastChanged();

protected:
	UPROPERTY(EditAnywhere)
	FGameplayTag AttributeTag;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BaseValue;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<int32, FAttributeModifier> Modifiers;

	UPROPERTY(VisibleAnywhere)
	float FinalValue = 0.0f;
	TMap<TWeakObjectPtr<UObject>, TFunction<void(const TWeakObjectPtr<UAttribute>)>> OnChangedCallbacks;
	int32 NextModifierId = 0;
};
