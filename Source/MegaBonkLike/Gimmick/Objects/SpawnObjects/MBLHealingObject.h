#pragma once

#include "CoreMinimal.h"
#include "Gimmick/Objects/SpawnObjects/MBLBaseSpawnObject.h"
#include "MBLHealingObject.generated.h"

UCLASS()
class MEGABONKLIKE_API AMBLHealingObject : public AMBLBaseSpawnObject
{
	GENERATED_BODY()

public:
	AMBLHealingObject();
	virtual void OnObjectActivated(AActor* Activator) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HealingObject", meta = (AllowPrivateAccess = "true"))
	int32 HealAmount;
};
