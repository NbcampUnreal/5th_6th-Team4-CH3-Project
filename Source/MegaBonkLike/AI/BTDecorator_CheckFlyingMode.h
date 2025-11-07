#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTDecorator_CheckFlyingMode.generated.h"

UCLASS()
class MEGABONKLIKE_API UBTDecorator_CheckFlyingMode : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
	
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
