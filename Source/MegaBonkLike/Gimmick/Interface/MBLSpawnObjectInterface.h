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
	// 플레이어가 오버랩됐을때 호출
	virtual void OnPlayerOverlapBegin(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	) = 0;
	// 오버랩이 끝났을때 호출
	virtual void OnPlayerOverlapEnd(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	) = 0;
	// 해당 아이템이 활성화 됐을때 호출
	virtual void OnObjectActivated() = 0;
	// 아이템 타입 Getter
	virtual FName GetObejctType() const = 0;
};
