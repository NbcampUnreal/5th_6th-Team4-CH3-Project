#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "Attribute/AttributeTags.h"
#include "SkillActionBase.generated.h"

struct FAttributeModifier;
class UWeaponItem;

UCLASS(Abstract, BlueprintType, EditInlineNew)
class MEGABONKLIKE_API USkillActionBase : public UObject
{
	GENERATED_BODY()
	
public:
	virtual void Activate(TWeakObjectPtr<AActor> InInstigator);
	virtual void Tick(float DeltaTime) {}
	virtual void Deactivate() {}

	void SetOwnerWeapon(TWeakObjectPtr<UWeaponItem> InOwnerWeapon);
	bool GetUseTick() const { return bUseTick; }

protected:
	void SetIntervalTimer();
	float GetAttributeValue(const FGameplayTag& AttributeTag);
	float GetWeaponValue(const FGameplayTag& AttributeTag);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadonly)
	bool bUseTick;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BaseTimerInterval;

	UPROPERTY()
	TWeakObjectPtr<AActor> Instigator;
	UPROPERTY()
	TWeakObjectPtr<UWeaponItem> OwnerWeapon;

	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate;
};
