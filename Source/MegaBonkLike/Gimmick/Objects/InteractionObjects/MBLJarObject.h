#pragma once

#include "CoreMinimal.h"
#include "Gimmick/Objects/InteractionObjects/MBLBaseInteractionObject.h"
#include "MBLJarObject.generated.h"

struct FSpawnObjectsSpawnRow;

UCLASS()
class MEGABONKLIKE_API AMBLJarObject : public AMBLBaseInteractionObject
{
	GENERATED_BODY()
	
public:
	AMBLJarObject();
	virtual void OnObjectActivated(AActor* Activator) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DropTable")
	UDataTable* DropTable;

	FSpawnObjectsSpawnRow* GetDropItem() const;
};
