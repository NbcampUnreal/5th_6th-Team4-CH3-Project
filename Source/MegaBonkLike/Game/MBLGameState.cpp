#include "Game/MBLGameState.h"
#include "Player/MBLPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Game/MBLGameMode.h"
#include "Gimmick/Data/MBLWaveEnums.h"
#include "Components/AudioComponent.h"

AMBLGameState::AMBLGameState()
	: WaveBGMs()
	, BGMComp(nullptr)
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
		if (!GameMode->OnWaveChanged.IsAlreadyBound(this, &AMBLGameState::PlayBGM))
		{
			GameMode->OnWaveChanged.AddDynamic(this, &AMBLGameState::PlayBGM);
		}
		WaveDuration = GameMode->GetWaveDuration();
	}
	StartWave();
	PlayBGM(EMBLWaveState::Wave1);
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

void AMBLGameState::PlayBGM(EMBLWaveState CurrentWave)
{
	if (WaveBGMs.IsEmpty()) return;

	BGMOff();

	uint8 BGMIndex = uint8(CurrentWave) - 1;
	uint8 FinalWave = uint8(EMBLWaveState::FinalWave);

	if (BGMIndex > FinalWave) return;
	
	BGMComp = UGameplayStatics::SpawnSound2D(this, WaveBGMs[BGMIndex]);
}

void AMBLGameState::BGMOff()
{
	if (BGMComp)
	{
		BGMComp->Stop();
		BGMComp->DestroyComponent();
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