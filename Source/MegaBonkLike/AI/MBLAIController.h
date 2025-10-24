#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MBLAIController.generated.h"

class UBlakcboardData;
class UBehaviorTree;

UCLASS()
class MEGABONKLIKE_API AMBLAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AMBLAIController();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void OnPossess(APawn* InPawn);

	void BeginAI(APawn* InPawn);
	void EndAI();

public:
	static const float PatrolRadius;

	static int32 ShowAIDebug;

	static const FName StartPatrolPositionKey;
	static const FName EndPatrolPositionKey;
	static const FName TargetCharacterKey;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UBlackboardData> BlackboardDataAsset;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UBehaviorTree> BehaviorTree;
};
