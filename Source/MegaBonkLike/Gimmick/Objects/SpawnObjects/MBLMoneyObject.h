#pragma once

#include "CoreMinimal.h"
#include "Gimmick/Objects/SpawnObjects/MBLBaseSpawnObject.h"
#include "MBLMoneyObject.generated.h"

UCLASS()
class MEGABONKLIKE_API AMBLMoneyObject : public AMBLBaseSpawnObject
{
	GENERATED_BODY()

public:
	AMBLMoneyObject();
	virtual void OnObjectActivated(AActor* Activator) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoneyObject", meta = (AllowPrivateAccess="true"))
	int32 MoneyValue;
	
};
