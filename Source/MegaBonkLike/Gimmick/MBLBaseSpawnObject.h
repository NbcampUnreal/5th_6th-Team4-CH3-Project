#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/MBLSpawnObjectInterface.h"
#include "MBLBaseSpawnObject.generated.h"

class USceneComponent;
class USphereComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;

UCLASS()
class MEGABONKLIKE_API AMBLBaseSpawnObject : public AActor, public IMBLSpawnObjectInterface
{
	GENERATED_BODY()
	
public:	
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
	virtual void CallOverlap(UPrimitiveComponent* CollisionComponent) override;
	virtual void OnObjectActivated(AActor* Activator) override;
	virtual FName GetObejctType() const override;
	virtual void DestroyObject() override;

protected:
	virtual void BeginPlay() override;

	// ������Ʈ
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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SpawnObject|Component")
	UProjectileMovementComponent* ProjectileComp;

	// ������Ʈ Rotation
	FTimerHandle RotationTimerHandle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnObject|Rotation")
	float RotationSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnObject|Rotation")
	float UpdateRotation;

	// ������Ʈ ����
	FTimerHandle ChaseTimerHandle;
	AActor* TargetActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnObject|ChaseValue")
	float BaseSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnObject|ChaseValue")
	float UpdateInterval;

	void RotationObject();
	void ChaseToPlayer();


};
