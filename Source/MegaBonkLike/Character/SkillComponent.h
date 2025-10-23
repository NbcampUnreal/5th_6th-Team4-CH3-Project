#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillComponent.generated.h"

class USkill;

UCLASS( ClassGroup=(Character), meta=(BlueprintSpawnableComponent) )
class MEGABONKLIKE_API USkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USkillComponent();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	int32 AddSkill(USkill* NewSkill);
	void RemoveSkill(int32 InSkillId);
	USkill* GetSkill(int32 InSKillId) const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<int32, TObjectPtr<USkill>> Skills;

	int32 NextId = 0;
};
