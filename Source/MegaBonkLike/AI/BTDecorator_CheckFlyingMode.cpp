#include "AI/BTDecorator_CheckFlyingMode.h"
#include "BehaviorTree/BlackboardComponent.h"

bool UBTDecorator_CheckFlyingMode::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	bool bCanFly = BB->GetValueAsBool(GetSelectedBlackboardKey());

	return false;
}
