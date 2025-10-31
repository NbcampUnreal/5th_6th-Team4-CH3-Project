#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GroundAttack.generated.h"

class USceneComponent;
class USphereComponent;

UCLASS()
class MEGABONKLIKE_API AGroundAttack : public AActor
{
	GENERATED_BODY()
	
public:	
	AGroundAttack();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> SceneComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent> SphereComponent;
};
