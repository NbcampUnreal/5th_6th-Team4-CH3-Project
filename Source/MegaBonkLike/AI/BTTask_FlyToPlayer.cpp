#include "AI/BTTask_FlyToPlayer.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/EnemyBase.h"
#include "Character/FlyingEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

UBTTask_FlyToPlayer::UBTTask_FlyToPlayer()
{
	NodeName = TEXT("Fly to Player");
	bNotifyTick = false;
}


EBTNodeResult::Type UBTTask_FlyToPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AICon = OwnerComp.GetAIOwner();
	if (!AICon) return EBTNodeResult::Failed;

	cachedEnemy = Cast<AFlyingEnemy>(AICon->GetPawn());
	if (!cachedEnemy) return EBTNodeResult::Failed;

	CachedOwnerComp = &OwnerComp;


	cachedEnemy->SetFlyingMode(true);
	

	cachedEnemy->GetWorldTimerManager().SetTimer(
		CheckHandle,
		this,
		&UBTTask_FlyToPlayer::CheckLandingCondition,
		0.3f,
		true
	);

	return EBTNodeResult::InProgress;
}

void UBTTask_FlyToPlayer::CheckLandingCondition()
{
	UWorld* World = cachedEnemy->GetWorld();
	if (!World) return;

	FHitResult HitResult;
	FVector Start = cachedEnemy->GetActorLocation();
	FVector End = Start - FVector(0, 0, 200.f);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(cachedEnemy);

	bool bHit = World->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_Visibility, // ¶Ç´Â ECC_WorldStatic
		Params
	);

	if (bHit)
	{
		cachedEnemy->GetWorldTimerManager().ClearTimer(CheckHandle);

		if (CachedOwnerComp)
		{
			FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
		}
	}

}