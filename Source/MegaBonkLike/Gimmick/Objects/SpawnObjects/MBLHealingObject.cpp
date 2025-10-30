#include "MBLHealingObject.h"

AMBLHealingObject::AMBLHealingObject()
	: HealAmount(50)
{
	SpawnObjectType = "HealingItem";
}

void AMBLHealingObject::OnObjectActivated(AActor* Activator)
{
	Super::OnObjectActivated(Activator);
	Super::DestroyObject();
}
