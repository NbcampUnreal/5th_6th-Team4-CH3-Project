#pragma once

#include "CoreMinimal.h"
#include "Gimmick/Objects/InteractionObjects/MBLChestObject.h"
#include "MBLFreeChestObject.generated.h"

UCLASS()
class MEGABONKLIKE_API AMBLFreeChestObject : public AMBLChestObject
{
	GENERATED_BODY()

private:

public:
	AMBLFreeChestObject();
	virtual void BeginPlay() override;
	virtual void OnObjectActivated(AActor* Activator) override;
	virtual void UpdateRequiredGold() override;
	void UpdateFreeText();
	
};
