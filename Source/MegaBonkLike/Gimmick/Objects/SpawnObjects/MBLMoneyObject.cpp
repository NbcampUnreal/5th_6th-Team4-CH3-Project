#include "MBLMoneyObject.h"
#include "Character/MBLPlayerCharacter.h"

AMBLMoneyObject::AMBLMoneyObject()
	: MoneyValue(1.0f) // ÀÓ½Ã °ª
{
	SpawnObjectType = "Money";
}

void AMBLMoneyObject::OnObjectActivated(AActor* Activator)
{
	if (AMBLPlayerCharacter* Player = Cast<AMBLPlayerCharacter>(Activator))
	{
		Player->AcquireGold(MoneyValue);
		Super::DestroyObject();
	}
}
