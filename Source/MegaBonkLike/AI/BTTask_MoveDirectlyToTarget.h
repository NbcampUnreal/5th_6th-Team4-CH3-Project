#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_MoveDirectlyToTarget.generated.h"

UCLASS()
class MEGABONKLIKE_API UBTTask_MoveDirectlyToTarget : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_MoveDirectlyToTarget();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:

	UPROPERTY(EditAnywhere, Category = "Movement")
	float AcceptanceRadius;

private:
	bool IsTargetReachable(ACharacter* AICharacter, AActor* TargetActor);
	bool IsPlayerOnGround(ACharacter* PlayerCharacter);
};
