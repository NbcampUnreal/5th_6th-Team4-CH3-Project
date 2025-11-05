#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Gimmick/Data/MBLWaveEnums.h"
#include "MBLGameMode.generated.h"

class AMBLSpawnVolume;
struct FInteractionObjectsRow;
struct FSpawnEnemyList;

UCLASS()
class MEGABONKLIKE_API AMBLGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AMBLGameMode();

protected:
	virtual void BeginPlay() override;

public:
	// 프로토 타입
	FTimerHandle SpawnTimerHandle;
	FTimerHandle NextWaveTimerHandle;
	FTimerHandle GameOverTimerHandle;
	EMBLWaveState CurrentWave;
	float WaveDuration;
	AMBLSpawnVolume* SpawnVolume;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test|ItemObjects")
	int32 MaxSpawnObject;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test|EnemyClass")
	TArray<TSubclassOf<AActor>> Enemy; //MonsterBase 로 받게 수정

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyTable")
	UDataTable* EnemyTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test|BossClass")
	TSubclassOf<AActor> Boss;
	float SpawnInterval;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test")
	uint8 MaxSpawnEnemy;
	int32 CurrentEnemy;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DropTable")
	UDataTable* DropTable;
	
	void SpawnManager();
	void SpawnBoss();
	FInteractionObjectsRow* GetDropObject() const;
	TSubclassOf<AActor> GetEnemyClass(EMBLWaveState Wave) const;

	void DeadPlayer(); // 플레이어 사망시
	void DeadEnemy(); // 적 사망시
	void DeadBoss(); // 보스 사망시
	// 제거 예정
	void Dead(AActor* DeadActor);

	void GameOver();

private:
	void StartWave();
	void NextWave(EMBLWaveState& Wave);

	inline EMBLWaveState GetNextWave(EMBLWaveState& Wave) const
	{
		switch (Wave)
		{
		case EMBLWaveState::SetWave: return EMBLWaveState::Wave1;
		case EMBLWaveState::Wave1: return EMBLWaveState::Wave2;
		case EMBLWaveState::Wave2: return EMBLWaveState::Wave3;
		case EMBLWaveState::Wave3: return EMBLWaveState::FinalWave;
		default: return EMBLWaveState::Finished;
		}
	}
};
