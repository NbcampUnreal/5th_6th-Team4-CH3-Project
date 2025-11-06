#include "Common/PoolSubsystem.h"
#include "Common/PoolBase.h"

void UPoolSubsystem::ReturnToPool(AActor* Actor)
{
	if (IsValid(Actor) == false)
		return;

	if (IPoolSpawnable* PoolSpawnable = Cast<IPoolSpawnable>(Actor))
	{
		PoolSpawnable->Deactivate();
	}

	TObjectPtr<UPoolBase>* PoolPtr = PoolMap.Find(Actor->GetClass());
	if (PoolPtr)
	{
		(*PoolPtr)->ReturnActor(Actor);
	}
	else
	{
		Actor->Destroy();
	}
}
