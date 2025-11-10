#include "MBLAIController.h"
#include "NavigationSystem.h"
#include "Navigation/NavLinkProxy.h"
#include "NavLinkComponent.h"
#include "EngineUtils.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/MBLNonPlayerCharacter.h"
#include "Character/FlyingEnemy.h"
#include "Character/MBLBossCharacter.h"
#include "Character/MBLEliteMonster.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

AMBLAIController::AMBLAIController()
{
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));
}

void AMBLAIController::BeginPlay()
{
	Super::BeginPlay();
}

void AMBLAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (InPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("AI Controller is Controlling %s."), *InPawn->GetName());
	}

	if (InPawn)
	{
		if (AMBLBossCharacter* Boss = Cast<AMBLBossCharacter>(InPawn))
		{
			//보스몬스터 BB/BT 실행
			UseBlackboard(BossBlackboardAsset, BlackboardComp);
			RunBehaviorTree(BossBehaviorTreeAsset);
		}

		else if (AMBLNonPlayerCharacter* Walk = Cast<AMBLNonPlayerCharacter>(InPawn))
		{
			//일반몬스터 BB/BT 실행
			BlackboardComp->SetValueAsBool(TEXT("CanFly"), false);
			StartBehaviorTree();
		}
		else if (AMBLEliteMonster* Elite = Cast<AMBLEliteMonster>(InPawn))
		{
			TWeakObjectPtr<AMBLEliteMonster> WeakElite = Elite;

			GetWorld()->GetTimerManager().SetTimerForNextTick([WeakElite]()
				{
					if (WeakElite.IsValid())
					{
						WeakElite->SpawnWeapon();
					}
				});

			BlackboardComp->SetValueAsBool(TEXT("CanFly"), false);
			StartBehaviorTree();
		}

		else if (AFlyingEnemy* Enemy = Cast<AFlyingEnemy>(InPawn))
		{
			BlackboardComp->SetValueAsBool(TEXT("CanFly"), true);
			BlackboardComp->SetValueAsBool(TEXT("IsFlyingMode"), true);
			StartBehaviorTree();
		}
	}
	GetWorld()->GetTimerManager().SetTimerForNextTick(
		this, 
		&AMBLAIController::SetPlayerPawnToBlackboard 
	);
}

void AMBLAIController::SetPlayerPawnToBlackboard()
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (PlayerPawn && GetBlackboardComp())
	{

		GetBlackboardComp()->SetValueAsObject(TEXT("TargetCharacter"), PlayerPawn);

	}
}

UBlackboardComponent* AMBLAIController::GetBlackboardComp() const
{
	return BlackboardComp;
}

void AMBLAIController::StartBehaviorTree()
{
	if (BehaviorTreeAsset)
	{
		RunBehaviorTree(BehaviorTreeAsset);
	}
	else
	{

	}
}