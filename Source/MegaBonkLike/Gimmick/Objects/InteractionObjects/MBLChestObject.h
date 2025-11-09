#pragma once

#include "CoreMinimal.h"
#include "Gimmick/Objects/InteractionObjects/MBLBaseInteractionObject.h"
#include "MBLChestObject.generated.h"

UCLASS()
class MEGABONKLIKE_API AMBLChestObject : public AMBLBaseInteractionObject
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DenySound", meta = (AllowPrivateAccess = "true"))
	USoundBase* DenySound;

public:
	AMBLChestObject();
	virtual void BeginPlay() override;
	virtual void OnObjectActivated(AActor* Activator) override;

	UFUNCTION()
	void UpdateRequiredGold();
	void DenyMessage();
};
