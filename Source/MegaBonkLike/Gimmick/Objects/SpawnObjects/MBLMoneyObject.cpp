#include "MBLMoneyObject.h"

AMBLMoneyObject::AMBLMoneyObject()
	: MoneyValue(1) // �ӽ� ��
{
	SpawnObjectType = "Money";
}

void AMBLMoneyObject::OnObjectActivated(AActor* Activator)
{
	Super::OnObjectActivated(Activator);
	Super::DestroyObject();
}
