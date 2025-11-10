#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "ElitePickupComponent.generated.h"

class AMBLNonPlayerCharacter;

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MEGABONKLIKE_API UElitePickupComponent : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:
	UElitePickupComponent();

protected:
	virtual void BeginPlay() override;


};
