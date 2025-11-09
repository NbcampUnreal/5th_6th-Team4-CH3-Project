#pragma once

#include "CoreMinimal.h"
#include "AttackData.generated.h"

USTRUCT(BlueprintType)
struct MEGABONKLIKE_API FAttackData
{
	GENERATED_BODY()

	UPROPERTY()
	float Damage;
	UPROPERTY()
	float CriticalChance;
	UPROPERTY()
	float CriticalMultiplier;
	UPROPERTY()
	float LifeSteal;
	UPROPERTY()
	float Knockback;
	UPROPERTY()
	FVector KnockbackDirection;

	UPROPERTY()
	TWeakObjectPtr<AActor> Causer;
};
