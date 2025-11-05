#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Attack/AttackData.h"
#include "AttackHandleComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MEGABONKLIKE_API UAttackHandleComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAttackHandleComponent();

	virtual void BeginPlay() override;

	float ExecuteAttack(AActor* Target, const FAttackData& Data);

private:
	void ApplyKnockback(AActor* Target, AActor* Causer, float KnockbackForce, const FVector& KnockbackDirection);
	void OnEndAIKnockback(class AAIController* AIController, FVector OriginalTargetLocation);
};
