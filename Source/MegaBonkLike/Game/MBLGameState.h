#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "MBLGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangedKillCount, int32, KillCount);

UCLASS()
class MEGABONKLIKE_API AMBLGameState : public AGameState
{
	GENERATED_BODY()
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

	UPROPERTY(BlueprintReadWrite, Category = "Coin")
	int32 CollectedCoinCount;

	UPROPERTY(BlueprintReadWrite, Category = "Kill")
	int32 KillCount;

	FTimerHandle WaveTimerHandle;

	void StartWave();
	void OnWaveEnd();
	void UpdateTimer();
	void Addkill();
	void AddCoin(int32 Amount);

	int32 GetKills() const;
	float GetRemainingTime() const;  //추가

	void UpdateHUD();
};
