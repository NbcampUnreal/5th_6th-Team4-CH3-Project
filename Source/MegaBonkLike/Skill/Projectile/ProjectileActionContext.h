#pragma once

#include "CoreMinimal.h"
#include "ProjectileActionContext.generated.h"

USTRUCT(BlueprintType)
struct MEGABONKLIKE_API FProjectileActionContext
{
	GENERATED_BODY()

	UPROPERTY()
	float LifeTime = 3.0f;

	UPROPERTY()
	int32 ChainCount = 1;

	UPROPERTY()
	TWeakObjectPtr<AActor> Instigator = nullptr;
};
