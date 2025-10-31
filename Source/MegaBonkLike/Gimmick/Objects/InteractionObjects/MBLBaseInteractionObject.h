#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gimmick/Objects/Interface/MBLSpawnObjectInterface.h"
#include "MBLBaseInteractionObject.generated.h"

class USceneComponent;
class USphereComponent;
class UStaticMeshComponent;
class UWidgetComponent;

UCLASS()
class MEGABONKLIKE_API AMBLBaseInteractionObject : public AActor, public IMBLSpawnObjectInterface
{
	GENERATED_BODY()
	
public:
	AMBLBaseInteractionObject();

protected:
	virtual void BeginPlay() override;

	// ������Ʈ Ÿ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InteractionObject")
	FName InteractionObjectType;

private:
	// ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "InteractionObject|Component", meta = (AllowPrivateAccess = "true"))
	USceneComponent* SceneComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "InteractionObject|Component", meta = (AllowPrivateAccess = "true"))
	USphereComponent* DetectionComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "InteractionObject|Component", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMeshComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "InteractionObject|Component", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* InteractableWidget;


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

};
