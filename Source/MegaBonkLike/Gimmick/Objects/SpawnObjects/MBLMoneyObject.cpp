#include "MBLMoneyObject.h"

AMBLMoneyObject::AMBLMoneyObject()
	: MoneyValue(1) // ÀÓ½Ã °ª
{
	SpawnObjectType = "Money";
}

void AMBLMoneyObject::OnObjectActivated(AActor* Activator)
{
	Super::OnObjectActivated(Activator);
	Super::DestroyObject();
}
