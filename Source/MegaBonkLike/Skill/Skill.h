#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Skill.generated.h"

class USkillActionBase;
class UItemBase;

UCLASS(BlueprintType, Blueprintable, Abstract)
class MEGABONKLIKE_API USkill : public UObject
{
	GENERATED_BODY()
	
public:
	void Activate(AActor* InInstigator);
	void Tick(float DeltaTime);
	void Deactivate();

	void SetOwnerWeapon(UItemBase* InOwnerWeapon);

protected:
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly)
	TArray<TObjectPtr<USkillActionBase>> SkillActions;
	UPROPERTY()
	TWeakObjectPtr<AActor> Instigator;
	UPROPERTY()
	TWeakObjectPtr<UItemBase> OwnerWeapon;
};
