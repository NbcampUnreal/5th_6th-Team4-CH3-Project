#include "MBLMoneyObject.h"
#include "Character/MBLPlayerCharacter.h"
#include "Game/MBLGameState.h"
#include "Kismet/GameplayStatics.h"

AMBLMoneyObject::AMBLMoneyObject()
	: MoneyValue(1.0f) // 임시 값
{
	SpawnObjectType = "Money";
}

void AMBLMoneyObject::OnObjectActivated(AActor* Activator)
{
	if (AMBLPlayerCharacter* Player = Cast<AMBLPlayerCharacter>(Activator))
	{	
		Player->AcquireGold(MoneyValue);
		Super::OnObjectActivated(Activator);
		Super::DestroyObject();
	}
}
