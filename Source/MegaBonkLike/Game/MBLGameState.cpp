#include "Game/MBLGameState.h"
#include "Player/MBLPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Game/MBLGameMode.h"
#include "Game/MBLGameState.h"  //추가

AMBLGameState::AMBLGameState()
{
	CurrentWaveIndex = 0;
	MaxWaves = 4;
	WaveDuration = 20.0f; //임시 60초로 할 예정
	RemainingTime = WaveDuration;
	CollectedCoinCount = 0;
	KillCount = 0;
}

void AMBLGameState::BeginPlay()
{
	Super::BeginPlay();
	StartWave();
}

void AMBLGameState::StartWave()
{	//float GetWaveDuration();

	/*if (AMBLGameMode* GameMode = Cast<AMBLGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		RemainingTime = GetWaveDuration();
	}*/

	RemainingTime = WaveDuration;
	GetWorldTimerManager().SetTimer(
		WaveTimerHandle,
		this,
		&AMBLGameState::UpdateTimer,
		1.0f,
		true
	);
}

void AMBLGameState::UpdateTimer()
{
	RemainingTime -= 1.0f;

	UpdateHUD();

	if (RemainingTime <= 0.0f)
	{
		GetWorldTimerManager().ClearTimer(WaveTimerHandle);
		OnWaveEnd();
	}
}

void AMBLGameState::OnWaveEnd()
{
	CurrentWaveIndex++;

	if (CurrentWaveIndex < MaxWaves - 1)
	{
		StartWave();
	}
	else if (CurrentWaveIndex == MaxWaves - 1)
	{
		UpdateHUD();
	}
	else
	{
		if (APlayerController* Controller = GetWorld()->GetFirstPlayerController())
		{
			if (AMBLPlayerController* MBLController = Cast<AMBLPlayerController>(Controller))
			{
				MBLController->ShowEndGameScreen(true);
			}
		}
	}
}

void AMBLGameState::Addkill()
{
	KillCount++;
	UpdateHUD();
}

void AMBLGameState::AddCoin(int32 Amount)
{
	CollectedCoinCount += Amount;
	UpdateHUD();
}

void AMBLGameState::UpdateHUD()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AMBLPlayerController* GameController = Cast<AMBLPlayerController>(PlayerController))
		{
			GameController->UpdateTimer(RemainingTime);
			GameController->UpdateCoinCount(CollectedCoinCount);
			GameController->UpdateKillCount(KillCount);
			
			if (CurrentWaveIndex < MaxWaves - 1)
			{
				GameController->UpdateWave(CurrentWaveIndex + 1, MaxWaves - 1);
			}
			else if (CurrentWaveIndex == MaxWaves - 1)
			{
				GameController->UpdateBossWaveText();
			}
			
		}
	}
}

int32 AMBLGameState::GetKills() const
{
	return KillCount;
}

float AMBLGameState::GetRemainingTime() const
{
	return RemainingTime;
}