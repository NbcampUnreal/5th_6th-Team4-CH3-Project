#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_RifleAttack.generated.h"

UCLASS()
class MEGABONKLIKE_API UBTTask_RifleAttack : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_RifleAttack();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

private:
	UPROPERTY()
	class AMBLEliteMonster* CachedEnemy;
	UPROPERTY()
	class UBehaviorTreeComponent* CachedOwnerComp;
};
