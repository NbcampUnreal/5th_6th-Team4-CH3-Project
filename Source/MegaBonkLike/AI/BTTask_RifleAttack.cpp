#include "AI/BTTask_RifleAttack.h"
#include "Character/MBLEliteMonster.h"
#include "AI/MBLAIController.h"
#include "Animation/Monster/EliteEnemyAnimInstance.h"

#include "BTTask_RifleAttack.h"

UBTTask_RifleAttack::UBTTask_RifleAttack()
{
	NodeName = TEXT("Play Attack Montage");
}

EBTNodeResult::Type UBTTask_RifleAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	/*AAIController* AICon = OwnerComp.GetAIOwner();
	if (!AICon) return EBTNodeResult::Failed;

	CachedEnemy = Cast<AMBLEliteMonster>(AICon->GetPawn());
	if (!CachedEnemy) return EBTNodeResult::Failed;

	UAnimInstance* AnimInstance = CachedEnemy->GetMesh()->GetAnimInstance();
	if (!AnimInstance || !CachedEnemy->AttackMontage) return EBTNodeResult::Failed;

	CachedOwnerComp = &OwnerComp;

	// 몽타주 끝나면 콜백 등록
	AnimInstance->OnMontageEnded.AddDynamic(this, &UBTTask_PlayAttackMontage::OnMontageEnded);

	CachedEnemy->PlayAttackMontage(); // 공격 시작

	// Task를 즉시 끝내지 않고 “진행 중” 상태로 유지
	return EBTNodeResult::InProgress;*/
	return EBTNodeResult::Succeeded;
}

void UBTTask_RifleAttack::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	/*if (!CachedEnemy || !CachedOwnerComp) return;

	UAnimInstance* AnimInstance = CachedEnemy->GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->OnMontageEnded.RemoveDynamic(this, &UBTTask_PlayAttackMontage::OnMontageEnded);
	}

	CachedOwnerComp->OnTaskFinished(this, EBTNodeResult::Succeeded);*/
}