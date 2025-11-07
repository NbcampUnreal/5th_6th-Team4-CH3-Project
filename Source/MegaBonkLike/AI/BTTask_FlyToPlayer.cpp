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
	FVector End = Start - FVector(0, 0, 200.f); // 발 밑 200cm까지 검사

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(cachedEnemy);

	bool bHit = World->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_Visibility, // 또는 ECC_WorldStatic
		Params
	);

	// 라인트레이스 디버그용 (확인용으로 그려보면 좋음)
	// DrawDebugLine(World, Start, End, FColor::Green, false, 0.1f, 0, 1.f);

	if (bHit)
	{
		// 실제로 바닥과 충돌했을 때만 착지로 간주
		cachedEnemy->SetFlyingMode(false);

		cachedEnemy->GetWorldTimerManager().ClearTimer(CheckHandle);

		if (CachedOwnerComp)
		{
			FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
		}
	}


	/*if (!cachedEnemy || !cachedEnemy->Target.IsValid()) return;

	const float MyZ = cachedEnemy->GetActorLocation().Z;
	const float TargetZ = cachedEnemy->Target->GetActorLocation().Z;
	const float ZDiff = FMath::Abs(TargetZ - MyZ);

	if (ZDiff < 10.f)
	{
		cachedEnemy->SetFlyingMode(false);

		cachedEnemy->GetWorldTimerManager().ClearTimer(CheckHandle);

		if (CachedOwnerComp)
		{
			FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
		}
	}*/
}