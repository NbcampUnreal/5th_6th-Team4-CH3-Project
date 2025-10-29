#include "MBLExpObject.h"

AMBLExpObject::AMBLExpObject()
	: Exp(1)
{
	SpawnObjectType = "Exp";
}

void AMBLExpObject::OnObjectActivated(AActor* Activator)
{
	Super::OnObjectActivated(Activator);
	Super::DestroyObject();
}
