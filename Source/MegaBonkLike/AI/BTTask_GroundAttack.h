#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_GroundAttack.generated.h"

UCLASS()
class MEGABONKLIKE_API UBTTask_GroundAttack : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_GroundAttack();

protected:
	
	TSubclassOf<class AMBLGroundAttack> GroundAttackClass;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
