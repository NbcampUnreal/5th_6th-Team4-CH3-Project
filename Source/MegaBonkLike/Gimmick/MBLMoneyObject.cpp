#include "Gimmick/MBLMoneyObject.h"

AMBLMoneyObject::AMBLMoneyObject()
	: MoneyValue(5) // �ӽ� ��
{
	SpawnObjectType = "Money";
}

void AMBLMoneyObject::OnObjectActivated(AActor* Activator)
{
	Super::OnObjectActivated(Activator);
	Super::DestroyObject();
}
