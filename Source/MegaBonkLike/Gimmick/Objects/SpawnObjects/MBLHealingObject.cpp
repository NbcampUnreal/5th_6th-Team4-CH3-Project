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
	if (AMBLPlayerCharacter* Actor = Cast<AMBLPlayerCharacter>(Activator))
	{
		if (AMBLCharacterBase* Player = Cast<AMBLCharacterBase>(Actor))
		{
			UE_LOG(LogTemp, Warning, TEXT("Heal"));
			Super::OnObjectActivated(Activator);
			Super::DestroyObject();
		}
	}
}
