#include "AI/BTTask_GroundAttack.h"
#include "AI/MBLAIController.h"
#include "Character/MBLBossCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TimerManager.h"

UBTTask_GroundAttack::UBTTask_GroundAttack()
{
    NodeName = TEXT("Ground Attack");
}

EBTNodeResult::Type UBTTask_GroundAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController) return EBTNodeResult::Failed;

    APawn* MyPawn = AIController->GetPawn();
    if(!MyPawn) return EBTNodeResult::Failed;

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if(!BlackboardComp) return EBTNodeResult::Failed;

    bool bIsAttacking = BlackboardComp->GetValueAsBool(TEXT("IsAttacking"));

    if (bIsAttacking)
    {
        return EBTNodeResult::Failed;
    }

    BlackboardComp->SetValueAsBool(TEXT("IsAttacking"), true);
    UE_LOG(LogTemp, Warning, TEXT("Boss Ground Attack Triggered!"));

    if (AMBLBossCharacter* Boss = Cast<AMBLBossCharacter>(MyPawn))
    {
        Boss->GroundAttack();
    }

    FTimerHandle CooldownHandle;
    FTimerDelegate CooldownDelegate;
    TWeakObjectPtr<UBlackboardComponent> WeakBlackboard = TWeakObjectPtr<UBlackboardComponent>(BlackboardComp);
    CooldownDelegate.BindLambda([WeakBlackboard]()
        {
            if (WeakBlackboard.IsValid() == false) return;

            WeakBlackboard->SetValueAsBool(TEXT("IsAttacking"), false);
            UE_LOG(LogTemp, Warning, TEXT("Ground Attack Cooldown Ended!"));
        });

    MyPawn->GetWorldTimerManager().SetTimer(
        CooldownHandle,
        CooldownDelegate,
        10.0f,
        false
    );

    return EBTNodeResult::Succeeded;
}
