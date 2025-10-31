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
	FTimerHandle GameOverTimerHandle;
	AMBLSpawnVolume* SpawnVolume;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyClass")
	TSubclassOf<AActor> Enemy;
	float SpawnInterval;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test")
	int32 MaxSpawnEnemy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test")
	float GameTime;
	int32 CurrentEnemy;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DropTable")
	UDataTable* DropTable;
	FInteractionObjectsRow* GetDropObject() const;
	void StartWave();
	void SpawnManager();
	void DeadEnemy();

	void GameOver();

};
