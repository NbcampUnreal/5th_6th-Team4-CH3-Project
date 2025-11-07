#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FlyToPlayer.generated.h"

class AEnemyBase;

UCLASS()
class MEGABONKLIKE_API UBTTask_FlyToPlayer : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_FlyToPlayer();
	UPROPERTY()
	class AFlyingEnemy* cachedEnemy;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	FTimerHandle CheckHandle;
	UPROPERTY()
	UBehaviorTreeComponent* CachedOwnerComp;
	UFUNCTION()
	void CheckLandingCondition();
};
