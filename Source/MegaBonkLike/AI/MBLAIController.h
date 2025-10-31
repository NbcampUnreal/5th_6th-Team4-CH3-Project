#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "MBLAIController.generated.h"


UCLASS()
class MEGABONKLIKE_API AMBLAIController : public AAIController
{
	GENERATED_BODY()

public:
	AMBLAIController();
	void StartBehaviorTree();

	UBlackboardComponent* GetBlackboardComp() const;

	UFUNCTION(BlueprintCallable)
	void OnSmartLinkJump(AActor* MovingActor, const FVector& DestinationPoint);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UBlackboardComponent* BlackboardComp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	UBlackboardData* BlackboardAsset;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	UBlackboardData* BossBlackboardAsset;
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBehaviorTree* BehaviorTreeAsset;
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBehaviorTree* BossBehaviorTreeAsset;

	UPROPERTY()
	AActor* CurrentTarget = nullptr;

	UFUNCTION()

	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

};
