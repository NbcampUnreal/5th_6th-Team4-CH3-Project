#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gimmick/Objects/Interface/MBLSpawnObjectInterface.h"
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

protected:
	virtual void BeginPlay() override;

	// 오브젝트 타입
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InteractionObject")
	FName InteractionObjectType;
	// 임시 드랍 아이템
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TEST")
	TSubclassOf<AActor> DropItem;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "InteractionObject|Component", meta = (AllowPrivateAccess = "true"))
	USceneComponent* SceneComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "InteractionObject|Component", meta = (AllowPrivateAccess = "true"))
	USphereComponent* DetectionComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "InteractionObject|Component", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMeshComp;

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
	UFUNCTION(BlueprintCallable)
	virtual void OnObjectActivated(AActor* Activator) override;
	virtual FName GetObejctType() const override;
	virtual void DestroyObject() override;
};
