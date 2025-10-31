#pragma once

#include "CoreMinimal.h"
#include "Gimmick/Objects/SpawnObjects/MBLBaseSpawnObject.h"
#include "MBLExpObject.generated.h"

UCLASS()
class MEGABONKLIKE_API AMBLExpObject : public AMBLBaseSpawnObject
{
	GENERATED_BODY()

public:
	AMBLExpObject();
	virtual void OnObjectActivated(AActor* Activator) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ExpObject", meta = (AllowPrivateAccess = "true"))
	float Exp;
};
