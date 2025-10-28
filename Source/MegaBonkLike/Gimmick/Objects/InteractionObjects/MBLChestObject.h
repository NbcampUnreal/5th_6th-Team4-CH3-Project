#pragma once

#include "CoreMinimal.h"
#include "Gimmick/Objects/InteractionObjects/MBLBaseInteractionObject.h"
#include "MBLChestObject.generated.h"

UCLASS()
class MEGABONKLIKE_API AMBLChestObject : public AMBLBaseInteractionObject
{
	GENERATED_BODY()
public:
	AMBLChestObject();
	virtual void OnObjectActivated(AActor* Activator) override;
};
