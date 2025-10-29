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
		UE_LOG(LogTemp, Warning, TEXT("Money Plus %.0f"), MoneyValue);
		Player->AcquireGold(MoneyValue);
		Super::DestroyObject();
	}
}
