// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/MBLSpawnObjectInterface.h"
#include "MBLBaseSpawnObject.generated.h"

class USceneComponent;
class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class MEGABONKLIKE_API AMBLBaseSpawnObject : public AActor, public IMBLSpawnObjectInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMBLBaseSpawnObject();

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
	virtual void OnObjectActivated(AActor* Activator) override;
	virtual FName GetObejctType() const override;
	virtual void DestroyObject() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnObject")
	FName SpawnObjectType;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SpawnObject|Component")
	USceneComponent* SceneComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SpawnObject|Component")
	USphereComponent* CollisionComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SpawnObject|Component")
	USphereComponent* DetectionComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SpawnObject|Component")
	UStaticMeshComponent* StaticMeshComp;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
