#include "Common/PoolSubsystem.h"
#include "Common/PoolBase.h"

void UPoolSubsystem::ReturnToPool(UObject* Object)
{
	if (IsValid(Object) == false)
		return;

	TObjectPtr<UPoolBase>* PoolPtr = PoolMap.Find(Object->GetClass());
	if (PoolPtr)
	{
		(*PoolPtr)->Return(Object);
	}
}
