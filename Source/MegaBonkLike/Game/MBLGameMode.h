#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MBLGameMode.generated.h"

class AMBLSpawnVolume;
struct FInteractionObjectsRow;

UCLASS()
class MEGABONKLIKE_API AMBLGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AMBLGameMode();
	virtual void BeginPlay() override;

	// 프로토 타입
	FTimerHandle SpawnTimerHandle;
	AMBLSpawnVolume* SpawnVolume;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyClass")
	TSubclassOf<AActor> Enemy;
	float SpawnInterval;
	int32 MaxSpawnEnemy;
	int32 CurrentEnemy;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DropTable")
	UDataTable* DropTable;
	FInteractionObjectsRow* GetDropObject() const;
	void StartWave();
	void SpawnManager();
	void DeadEnemy();

	void GameOver();

};
