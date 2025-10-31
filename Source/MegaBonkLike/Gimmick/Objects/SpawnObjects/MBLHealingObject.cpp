#include "MBLHealingObject.h"
#include "Character/MBLCharacterBase.h"
#include "Character/MBLPlayerCharacter.h"

AMBLHealingObject::AMBLHealingObject()
	: HealAmount(50.0f) // ÀÓ½Ã °ª
{
	SpawnObjectType = "HealingItem";
}

void AMBLHealingObject::OnObjectActivated(AActor* Activator)
{
	if (AMBLPlayerCharacter* Player = Cast<AMBLPlayerCharacter>(Activator))
	{
		Player->AddHealth(HealAmount);
		Super::OnObjectActivated(Activator);
		Super::DestroyObject();
	}
}
