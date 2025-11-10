#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Gimmick/Data/MBLWaveEnums.h"
#include "Character/EnemyBase.h"
#include "MBLGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWaveChanged, EMBLWaveState, Wave);

class AMBLSpawnVolume;
struct FInteractionObjectsRow;
struct FSpawnEnemyList;

UCLASS()
class MEGABONKLIKE_API AMBLGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AMBLGameMode();
	UPROPERTY(BlueprintAssignable)
	FOnWaveChanged OnWaveChanged;
	void SpawnBoss();
	FInteractionObjectsRow* GetDropObject() const;
	TSubclassOf<AEnemyBase> GetEnemyClass(EMBLWaveState Wave) const;
	void DeadPlayer(); // 플레이어 사망시
	void DeadEnemy(); // 적 사망시
	void DeadBoss(); // 보스 사망시
	float GetWaveDuration() const;
	float GetBossWaveDuration() const;
	void GameOver();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	AMBLSpawnVolume* SpawnVolume;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MaxSpawnObjects", meta = (AllowPrivateAccess = "true"))
	int32 MaxSpawnObject;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyTable", meta = (AllowPrivateAccess = "true"))
	UDataTable* EnemyTable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossClass", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AEnemyBase> Boss;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DropTable", meta = (AllowPrivateAccess = "true"))
	UDataTable* DropTable;
	int32 MaxSpawnEnemy;
	int32 CurrentEnemy;
	FTimerHandle SpawnTimerHandle;
	FTimerHandle NextWaveTimerHandle;
	FTimerHandle GameOverTimerHandle;
	EMBLWaveState CurrentWave;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Test|WaveDuration", meta = (AllowPrivateAccess = "true"))
	float WaveDuration;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Test|BossWaveDuration", meta = (AllowPrivateAccess = "true"))
	float BossWaveDuration;
	float SpawnInterval;

	void WaveSet();
	void SpawnManager();

	inline void NextWave()
	{
		switch (CurrentWave)
		{
		case EMBLWaveState::SetWave:
			CurrentWave = EMBLWaveState::Wave1;
			return;
		case EMBLWaveState::Wave1:
			CurrentWave = EMBLWaveState::Wave2;
			return;
		case EMBLWaveState::Wave2:
			CurrentWave = EMBLWaveState::Wave3;
			return;
		case EMBLWaveState::Wave3:
			CurrentWave = EMBLWaveState::FinalWave;
			return;
		default:
			CurrentWave = EMBLWaveState::Finished;
			return;
		}
	}
};
