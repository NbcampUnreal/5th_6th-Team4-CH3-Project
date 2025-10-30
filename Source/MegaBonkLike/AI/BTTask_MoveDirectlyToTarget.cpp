#include "AI/BTTask_MoveDirectlyToTarget.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"


UBTTask_MoveDirectlyToTarget::UBTTask_MoveDirectlyToTarget()
{
	NodeName = TEXT("Move Directly To Target");

    bNotifyTick = true;
    AcceptanceRadius = 0.f;
}

EBTNodeResult::Type UBTTask_MoveDirectlyToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    return EBTNodeResult::InProgress;
}

void UBTTask_MoveDirectlyToTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    ACharacter* AIPawn = Cast<ACharacter>(AIController->GetPawn());
    if (!AIPawn)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TEXT("TargetCharacter")));
    if (!TargetActor)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    FVector AIPos = AIPawn->GetActorLocation();
    FVector TargetPos = TargetActor->GetActorLocation();
    float HeightDiff = TargetPos.Z - AIPos.Z;
    float Distance = FVector::Dist(TargetPos, AIPos);

    ACharacter* PlayerCharacter = Cast<ACharacter>(TargetActor);
    bool bPlayerOnGround = PlayerCharacter ? IsPlayerOnGround(PlayerCharacter) : true;

    if (!bPlayerOnGround)
    {
        AIController->StopMovement(); // NavMesh MoveTo Áß´Ü
        FVector Direction = (TargetPos - AIPos).GetSafeNormal();
        AIPawn->AddMovementInput(Direction, 1.0f);
    }
    else
    {
        AIController->MoveToActor(TargetActor, AcceptanceRadius);
    }
}

bool UBTTask_MoveDirectlyToTarget::IsTargetReachable(ACharacter* AICharacter, AActor* TargetActor)
{
    if (!AICharacter || !TargetActor) return false;

    UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(AICharacter->GetWorld());
    if (!NavSystem) return false;

    ANavigationData* NavData = NavSystem->GetDefaultNavDataInstance(FNavigationSystem::DontCreate);
    if (!NavData) return false;

    FPathFindingQuery Query;
    Query = FPathFindingQuery(AICharacter, *NavData, AICharacter->GetActorLocation(), TargetActor->GetActorLocation());

    FPathFindingResult Result = NavSystem->FindPathSync(Query);
    return Result.IsSuccessful();
}

bool UBTTask_MoveDirectlyToTarget::IsPlayerOnGround(ACharacter* PlayerCharacter)
{
    if (!PlayerCharacter) return true;
    
    return PlayerCharacter->GetCharacterMovement()->IsMovingOnGround();
}

