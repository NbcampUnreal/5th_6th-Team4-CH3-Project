#include "Game/MBLGameMode.h"
#include "Player/MBLPlayerController.h"

void AMBLGameMode::BeginPlay()
{
    Super::BeginPlay();

    //GetWorldTimerManager().SetTimer(
    //    GameOverTimerHandle,
    //    this,
    //    &AMBLGameMode::GameOver,
    //    10.0f,
    //    false
    //);
}

void AMBLGameMode::GameOver()
{
    //if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
    //{
    //    if (AMBLPlayerController* MBLPlayerController = Cast<AMBLPlayerController>(PlayerController))
    //    {
    //        MBLPlayerController->SetPause(true);
    //        MBLPlayerController->ShowEndGameScreen(true);
    //    }
    //}
}