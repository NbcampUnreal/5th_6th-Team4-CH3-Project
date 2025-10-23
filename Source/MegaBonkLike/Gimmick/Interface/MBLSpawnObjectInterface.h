// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MBLSpawnObjectInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMBLSpawnObjectInterface : public UInterface
{
	GENERATED_BODY()
};

class MEGABONKLIKE_API IMBLSpawnObjectInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
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
	// ������Ʈ ��� �Լ�
	virtual void OnObjectActivated(AActor* Activator) = 0;
	// ������Ʈ Ÿ�� Getter
	virtual FName GetObejctType() const = 0;
	// ������Ʈ ���� �Լ�
	virtual void DestroyObject() = 0;
};
