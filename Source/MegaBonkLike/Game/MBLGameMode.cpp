#include "Game/MBLGameMode.h"
#include "Player/MBLPlayerController.h"
#include "Gimmick/Spawn/MBLSpawnVolume.h"

void AMBLGameMode::BeginPlay()
{
    Super::BeginPlay();

    GetWorldTimerManager().SetTimer(
        SpawnTimerHandle,
        this,
        &AMBLGameMode::SpawnManager,
        1.0f,
        true
    );


}

void AMBLGameMode::StartWave()
{
}

void AMBLGameMode::SpawnManager()
{
    
}

void AMBLGameMode::GameOver()
{
    if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
    {
        if (AMBLPlayerController* MBLPlayerController = Cast<AMBLPlayerController>(PlayerController))
        {
            MBLPlayerController->SetPause(true);
            MBLPlayerController->ShowEndGameScreen(true);
        }
    }
}