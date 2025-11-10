#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EliteWeapon.generated.h"

class AMBLNonPlayerCharacter;
UCLASS()
class MEGABONKLIKE_API AEliteWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AEliteWeapon();

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

};
