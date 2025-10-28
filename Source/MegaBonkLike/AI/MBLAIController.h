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
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBehaviorTree* BehaviorTreeAsset;

	UPROPERTY()
	AActor* CurrentTarget = nullptr;

	UFUNCTION()

	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

};
