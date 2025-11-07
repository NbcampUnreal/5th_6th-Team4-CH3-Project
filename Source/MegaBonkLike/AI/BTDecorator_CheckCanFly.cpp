#include "AI/BTDecorator_CheckCanFly.h"
#include "BehaviorTree/BlackboardComponent.h"

bool UBTDecorator_CheckCanFly::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	bool bCanFly = BB->GetValueAsBool(GetSelectedBlackboardKey());

	//UE_LOG(LogTemp, Warning, TEXT("[CheckCanFly] Key: %s, Value: %s"),*GetSelectedBlackboardKey().ToString(),	bCanFly ? TEXT("TRUE") : TEXT("FALSE"));

	return bCanFly;
}
