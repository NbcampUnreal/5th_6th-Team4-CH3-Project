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

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawn")
	USceneComponent* Scene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawn")
	UBoxComponent* SpawnBox;

	UFUNCTION(BlueprintCallable, Category = "Spawn")
	FVector GetRandomEnemySpawnLocation() const;
	UFUNCTION(BlueprintCallable, Category = "Spawn")
	FVector GetRandomObjectSpawnLocation() const;
	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void SpawnEnemy(TSubclassOf<AActor> EnemyClass);
	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void SpawnObject(TSubclassOf<AActor> ObjectClass);
	UFUNCTION(BlueprintCallable, Category = "Spawn")
	AActor* SpawnActorAtLocation(
		TSubclassOf<AActor> ActorClass,
		const FVector& Location,
		const FRotator& Rotation
	);

private:
	const float SearchRadius = 100.f;
	AActor* GetPlayerInBox() const;
	FVector GetValidNavMeshLocation(const FVector& Location, float Radius) const;

};
