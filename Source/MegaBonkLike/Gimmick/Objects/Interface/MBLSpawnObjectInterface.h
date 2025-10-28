#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MBLSpawnObjectInterface.generated.h"

UINTERFACE(MinimalAPI)
class UMBLSpawnObjectInterface : public UInterface
{
	GENERATED_BODY()
};

class MEGABONKLIKE_API IMBLSpawnObjectInterface
{
	GENERATED_BODY()

public:
	// ������ �Լ�
	UFUNCTION()
	virtual void OnPlayerOverlapBegin(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	) = 0;
	// ������ ���� �Լ�
	UFUNCTION()
	virtual void OnPlayerOverlapEnd(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	) = 0;
	// ������ ���� ȣ�� �Լ�
	virtual void CallOverlap(UPrimitiveComponent* CollisionComponent) = 0;
	// ������Ʈ ��� �Լ�
	virtual void OnObjectActivated(AActor* Activator) = 0;
	// ������Ʈ Ÿ�� Getter
	virtual FName GetObejctType() const = 0;
	// ������Ʈ ���� �Լ�
	virtual void DestroyObject() = 0;
};
