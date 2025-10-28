#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MBLSpawnVolume.generated.h"

class USceneComponent;
class UBoxComponent;

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
	UFUNCTION(BlueprintCallable, Category = "Spawn")
	FVector GetRandomEnemySpawnLocation() const;
	UFUNCTION(BlueprintCallable, Category = "Spawn")
	FVector GetRandomObjectSpawnLocation() const;
	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void SpawnEnemy(TSubclassOf<AActor> EnemyClass);
	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void SpawnObject(TSubclassOf<AActor> ObjectClass);

private:
	const float SearchRadius = 100.f;
	AActor* GetPlayerInBox() const;
	FVector GetValidNavMeshLocation(const FVector& Location, float Radius) const;

};
