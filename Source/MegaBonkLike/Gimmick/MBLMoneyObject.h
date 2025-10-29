#pragma once

#include "CoreMinimal.h"
#include "Gimmick/MBLBaseSpawnObject.h"
#include "MBLMoneyObject.generated.h"

UCLASS()
class MEGABONKLIKE_API AMBLMoneyObject : public AMBLBaseSpawnObject
{
	GENERATED_BODY()

public:
	AMBLMoneyObject();
	virtual void OnObjectActivated(AActor* Activator) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoneyObject")
	int32 MoneyValue;
	
};
