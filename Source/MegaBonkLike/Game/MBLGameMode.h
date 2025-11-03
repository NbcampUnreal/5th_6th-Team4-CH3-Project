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
	FTimerHandle NextWaveTimerHandle;
	FTimerHandle GameOverTimerHandle;
	int32 CurrentWave;
	int32 MaxWave;
	float WaveDuration;
	bool bSpawnBoss;
	AMBLSpawnVolume* SpawnVolume;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test|ItemObjects")
	int32 MaxSpawnObject;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test|EnemyClass")
	TSubclassOf<AActor> Enemy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test|BossClass")
	TSubclassOf<AActor> Boss;
	float SpawnInterval;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test")
	int32 MaxSpawnEnemy;
	int32 CurrentEnemy;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DropTable")
	UDataTable* DropTable;
	FInteractionObjectsRow* GetDropObject() const;
	void StartWave();
	void SpawnBoss();
	void SpawnManager();
	void Dead(AActor* DeadActor);

	void GameOver();

};
