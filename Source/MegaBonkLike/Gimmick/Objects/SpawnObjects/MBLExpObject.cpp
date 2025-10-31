#include "MBLExpObject.h"
#include "Character/MBLPlayerCharacter.h"

AMBLExpObject::AMBLExpObject()
	: Exp(1.0f) // ÀÓ½Ã °ª
{
	SpawnObjectType = "Exp";
}

void AMBLExpObject::OnObjectActivated(AActor* Activator)
{
	if (AMBLPlayerCharacter* Player = Cast<AMBLPlayerCharacter>(Activator))
	{
		UE_LOG(LogTemp, Warning, TEXT("Player AcquireExp %.0f"), Exp);
		Player->AcquireExp(Exp);
		Super::DestroyObject();
	}
}
