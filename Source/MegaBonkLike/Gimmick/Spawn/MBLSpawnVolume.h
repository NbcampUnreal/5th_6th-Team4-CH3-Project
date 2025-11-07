#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MBLSpawnVolume.generated.h"

class USceneComponent;
class UBoxComponent;
class AEnemyBase;

UCLASS()
class MEGABONKLIKE_API AMBLSpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	AMBLSpawnVolume();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawn", meta = (AllowPrivateAccess = "true"))
	USceneComponent* Scene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawn", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* SpawnBox;

public:
	FVector GetRandomEnemySpawnLocation(float CapsuleHalfHeight) const;
	FVector GetRandomObjectSpawnLocation() const;
	AEnemyBase* SpawnEnemy(TSubclassOf<AEnemyBase> EnemyClass);
	void SpawnObject(TSubclassOf<AActor> ObjectClass);

private:
	const float SearchRadius = 100.f;
	AActor* GetPlayerInBox() const;
	FVector GetValidNavMeshLocation(const FVector& Location, float Radius, float CapsuleHalfHeight = 0.0f) const;

};
