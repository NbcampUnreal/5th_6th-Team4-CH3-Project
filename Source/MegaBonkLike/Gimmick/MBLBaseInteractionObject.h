#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/MBLSpawnObjectInterface.h"
#include "MBLBaseInteractionObject.generated.h"

class USceneComponent;
class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class MEGABONKLIKE_API AMBLBaseInteractionObject : public AActor, public IMBLSpawnObjectInterface
{
	GENERATED_BODY()
	
public:
	AMBLBaseInteractionObject();

	virtual void OnPlayerOverlapBegin(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	) override;
	virtual void OnPlayerOverlapEnd(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	) override;
	virtual void CallOverlap(UPrimitiveComponent* CollisionComponent) override;
	virtual void OnObjectActivated(AActor* Activator) override;
	virtual FName GetObejctType() const override;
	virtual void DestroyObject() override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InteractionObject")
	FName InteractionObjectType;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "InteractionObject|Component")
	USceneComponent* SceneComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "InteractionObject|Component")
	USphereComponent* DetectionComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "InteractionObject|Component")
	UStaticMeshComponent* StaticMeshComp;

public:
	virtual void Tick(float DeltaTime) override;

};
