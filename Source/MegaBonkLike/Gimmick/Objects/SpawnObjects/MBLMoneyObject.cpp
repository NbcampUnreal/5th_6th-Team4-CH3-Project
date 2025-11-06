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
		//임시
		if (AMBLGameState* GameState = GetWorld()->GetGameState<AMBLGameState>())
		{
			GameState->AddCoin(1);
		}
		//임시 

		Player->AcquireGold(MoneyValue);
		Super::DestroyObject();
	}
}
