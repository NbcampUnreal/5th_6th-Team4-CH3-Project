#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PoolSpawnable.generated.h"

UINTERFACE(MinimalAPI)
class UPoolSpawnable : public UInterface
{
	GENERATED_BODY()
};

class MEGABONKLIKE_API IPoolSpawnable
{
	GENERATED_BODY()

public:
	virtual void Activate() = 0;
	virtual void Deactivate() = 0;
};
