#include "Common/PoolBase.h"

void UPoolBase::Initialize(const TSubclassOf<UObject>& InClass, int32 InitialSize)
{
	ObjectClass = InClass;
}
