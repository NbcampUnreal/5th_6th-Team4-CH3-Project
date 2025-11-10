#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "MBLGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangedKillCount, int32, KillCount);

enum class EMBLWaveState : uint8;

UCLASS()
class MEGABONKLIKE_API AMBLGameState : public AGameState
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "BGM", meta = (AllowPrivateAccess = "true"))
	TArray<USoundBase*> WaveBGMs;
	UPROPERTY()
	UAudioComponent* BGMComp;

public:
	AMBLGameState();
	FOnChangedKillCount OnChangedKillCount;

protected:

	virtual void BeginPlay() override;

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Wave")
	int32 CurrentWaveIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	int32 MaxWaves;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	float WaveDuration;

	UPROPERTY(BlueprintReadWrite, Category = "Timer")
	float RemainingTime;

	UPROPERTY(BlueprintReadWrite, Category = "Kill")
	int32 KillCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float PlaryTime;

	UPROPERTY(BlueprintReadOnly)
	bool GmaeClear = false;

	FTimerHandle WaveTimerHandle;

	
	void ChangeBGM(EMBLWaveState CurrentWave);

	void StartWave();
	void OnWaveEnd();
	void UpdateTimer();
	void Addkill();

	int32 GetKills() const;
	float TimeSurvived() const;
	void UpdateHUD();
	UFUNCTION()
	void PlayBGM(EMBLWaveState CurrentWave);
	void BGMOff();
};
