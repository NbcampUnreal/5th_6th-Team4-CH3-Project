#include "Attack/AttackHandleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "BrainComponent.h"

UAttackHandleComponent::UAttackHandleComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAttackHandleComponent::BeginPlay()
{
	Super::BeginPlay();
}

float UAttackHandleComponent::ExecuteAttack(AActor* Target, const FAttackData& Data)
{
	if (IsValid(Target) == false || Data.Causer.IsValid() == false)
		return 0.0f;

	AActor* Causer = Data.Causer.Get();

	bool bIsCritical = FMath::RandRange(0.0f, 1.0f) < Data.CriticalChance;
	float FinalDamage = Data.Damage * (bIsCritical ? Data.CriticalMultiplier : 1.0f);

	UGameplayStatics::ApplyDamage(Target, FinalDamage, Data.Causer->GetInstigatorController(), Causer, nullptr);

	if (Data.Knockback > 0.0f)
	{
		ApplyKnockback(Target, Causer, Data.Knockback, Data.KnockbackDirection);
	}

	return FinalDamage;
}

void UAttackHandleComponent::ApplyKnockback(AActor* Target, AActor* Causer, float KnockbackForce, const FVector& KnockbackDirection)
{
	ACharacter* TargetCharacter = Cast<ACharacter>(Target);
	if (TargetCharacter == nullptr)
		return;

	if (AAIController* AIController = Cast<AAIController>(TargetCharacter->GetController()))
	{
		if (UBrainComponent* BrainComponent = AIController->GetBrainComponent())
		{
			BrainComponent->StopLogic(TEXT("Knockback"));

			FVector TargetLocation = Causer->GetActorLocation();
			FTimerHandle KnockbackTimerHandle;
			FTimerDelegate Delegate;
			Delegate.BindLambda(
				[this, AIController, TargetLocation]()
				{
					if (IsValid(this) == true && IsValid(AIController))
					{
						this->OnEndAIKnockback(AIController, TargetLocation);
					}
				});
			UWorld* World = GetWorld();
			if (World)
			{
				World->GetTimerManager().SetTimer(
					KnockbackTimerHandle,
					Delegate,
					0.3f,
					false);
			}
		}
	}

	FVector Direction = KnockbackDirection;
	Direction.Z += 0.2f;
	Direction = Direction.GetSafeNormal();
	TargetCharacter->LaunchCharacter(KnockbackForce * Direction, true, false);
}

void UAttackHandleComponent::OnEndAIKnockback(AAIController* AIController, FVector OriginalTargetLocation)
{
	if (IsValid(AIController) == false)
		return;

	if (UBrainComponent* BrainComponent = AIController->GetBrainComponent())
	{
		BrainComponent->StartLogic();
	}
}

