#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "MBLGameState.generated.h"

UCLASS()
class MEGABONKLIKE_API AMBLGameState : public AGameState
{
	GENERATED_BODY()
public:

	AMBLGameState();

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

	void UpdateHUD();
};
