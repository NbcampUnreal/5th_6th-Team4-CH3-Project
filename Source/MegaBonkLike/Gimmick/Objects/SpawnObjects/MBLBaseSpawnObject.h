#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gimmick/Objects/Interface/MBLSpawnObjectInterface.h"
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

protected:
	virtual void BeginPlay() override;

	// 오브젝트 타입
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnObject")
	FName SpawnObjectType;

private:
	// 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SpawnObject|Component", meta = (AllowPrivateAccess = "true"))
	USceneComponent* SceneComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SpawnObject|Component", meta = (AllowPrivateAccess = "true"))
	USphereComponent* CollisionComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SpawnObject|Component", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMeshComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SpawnObject|Component", meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileComp;

	// 오브젝트 Rotation
	FTimerHandle RotationTimerHandle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnObject|Rotation", meta = (AllowPrivateAccess = "true"))
	float RotationSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnObject|Rotation", meta = (AllowPrivateAccess = "true"))
	float UpdateRotation;

	// 오브젝트 추적
	FTimerHandle ChaseTimerHandle;
	AActor* TargetActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnObject|ChaseValue", meta = (AllowPrivateAccess = "true"))
	float BaseSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnObject|ChaseValue", meta = (AllowPrivateAccess = "true"))
	float UpdateInterval;

public:
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
	void SetTarget(AActor* Target);
private:
	void RotationObject();
	void ChaseToPlayer();
};
