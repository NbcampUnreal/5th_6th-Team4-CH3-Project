#include "MBLAIController.h"
#include "NavigationSystem.h"
#include "Navigation/NavLinkProxy.h"
#include "NavLinkComponent.h"
#include "EngineUtils.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/MBLNonPlayerCharacter.h"
#include "Character/FlyingEnemy.h"
#include "Character/MBLBossCharacter.h"
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

		if (AFlyingEnemy* Enemy = Cast<AFlyingEnemy>(InPawn))
		{
			BlackboardComp->SetValueAsBool(TEXT("CanFly"), true);
			BlackboardComp->SetValueAsBool(TEXT("IsFlyingMode"), true);
			StartBehaviorTree();
		}

	}

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

void AMBLAIController::OnSmartLinkJump(AActor* MovingActor, const FVector& DestinationPoint)
{
	AMBLNonPlayerCharacter* NPC = Cast<AMBLNonPlayerCharacter>(MovingActor);
	if (NPC)
	{
		StopMovement();
		NPC->GetCharacterMovement()->SetMovementMode(MOVE_Walking);

		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		if (PlayerPawn)
		{
			MoveToActor(PlayerPawn, 50.f);
		}
	}
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

