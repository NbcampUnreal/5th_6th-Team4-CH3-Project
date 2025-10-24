#include "Gimmick/MBLMoneyObject.h"

AMBLMoneyObject::AMBLMoneyObject()
	: MoneyValue(5) // ÀÓ½Ã °ª
{
	SpawnObjectType = "Money";
}

void AMBLMoneyObject::OnObjectActivated(AActor* Activator)
{
	Super::OnObjectActivated(Activator);
	Super::DestroyObject();
}
