#include "MBLAIController.h"
#include "NavigationSystem.h"
#include "Navigation/NavLinkProxy.h"
#include "NavLinkComponent.h"
#include "EngineUtils.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/MBLNonPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

AMBLAIController::AMBLAIController()
{
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));
}

void AMBLAIController::BeginPlay()
{
	Super::BeginPlay();

	StartBehaviorTree();

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (PlayerPawn && GetBlackboardComp())
	{

		GetBlackboardComp()->SetValueAsObject(TEXT("TargetCharacter"), PlayerPawn);
	}
}

void AMBLAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (InPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("AI Controller is Controlling %s."), *InPawn->GetName());
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
		NPC->HandleNavLinkJump(DestinationPoint);

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

