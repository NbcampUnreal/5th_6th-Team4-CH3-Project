// Fill out your copyright notice in the Description page of Project Settings.

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

	// TimerHandle
	FTimerHandle WaveTimerHandle;
	FTimerHandle HUDUpdateTimerHandle;

	// GameResult
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameResult")
	int32 KillCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameResult")
	float TimeAttack;

	// Wave
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	float WaveDuration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	int32 MaxWave;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
	int32 CurrentWaveIndex;

	// Spawn

	// ÇÔ¼ö
	void StartWave();
	void EndWave();
	void UpdateHUD();

protected:
	virtual void BeginPlay() override;
};
