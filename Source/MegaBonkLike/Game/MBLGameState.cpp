#include "Game/MBLGameState.h"
#include "Player/MBLPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Game/MBLGameMode.h"


AMBLGameState::AMBLGameState()
{
	CurrentWaveIndex = 0;
	MaxWaves = 4;
	WaveDuration = 0;
	RemainingTime = 0;
	PlaryTime = 0;
	KillCount = 0;
}

void AMBLGameState::BeginPlay()
{
	Super::BeginPlay();
	if (AMBLGameMode* GameMode = Cast<AMBLGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		WaveDuration = GameMode->GetWaveDuration();
	}
	StartWave();
}

void AMBLGameState::StartWave()
{	
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
	PlaryTime += 1.0f;

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

	StartWave();
		

}

void AMBLGameState::Addkill()
{
	KillCount++;

	OnChangedKillCount.Broadcast(KillCount);
	UpdateHUD();
}

void AMBLGameState::UpdateHUD()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AMBLPlayerController* GameController = Cast<AMBLPlayerController>(PlayerController))
		{
			GameController->UpdateTimer(RemainingTime);
			GameController->UpdateKillCount(KillCount);
			
			if (CurrentWaveIndex < MaxWaves - 1)
			{
				GameController->UpdateWave(CurrentWaveIndex + 1 , MaxWaves - 1);
			}
			else 
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

float AMBLGameState::TimeSurvived() const
{
	return PlaryTime;
}