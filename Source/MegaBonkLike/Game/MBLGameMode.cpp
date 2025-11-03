#include "Game/MBLGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Character/MBLPlayerCharacter.h"
#include "Character/MBLNonPlayerCharacter.h"
#include "Character/MBLBossCharacter.h"
#include "Player/MBLPlayerController.h"
#include "Gimmick/Spawn/MBLSpawnVolume.h"
#include "Gimmick/Data/InteractionObjectsRow.h"
#include "Character/MBLBossCharacter.h"

AMBLGameMode::AMBLGameMode()
    : SpawnVolume(nullptr)
    , CurrentWave(0)
    , WaveDuration(30.0f)
    , MaxWave(3)
    , MaxSpawnObject(50)
    , Enemy(nullptr)
    , Boss(nullptr)
    , SpawnInterval(2.0f)
    , MaxSpawnEnemy(10)
    , CurrentEnemy(0)
    , DropTable(nullptr)
    , bSpawnBoss(false)
{
    DefaultPawnClass = AMBLPlayerCharacter::StaticClass();
    PlayerControllerClass = AMBLPlayerController::StaticClass();
}

void AMBLGameMode::BeginPlay()
{
    Super::BeginPlay();

    SpawnVolume = Cast<AMBLSpawnVolume>(UGameplayStatics::GetActorOfClass(GetWorld(), AMBLSpawnVolume::StaticClass()));

    if (!IsValid(SpawnVolume))
    {
        UE_LOG(LogTemp, Error, TEXT("SpawnVolume not found"));
    }

    StartWave();

    GetWorldTimerManager().SetTimer(
        NextWaveTimerHandle,
        this,
        &AMBLGameMode::StartWave,
        WaveDuration,
        true
    );
}

FInteractionObjectsRow* AMBLGameMode::GetDropObject() const
{
    if (!DropTable) return nullptr;

    TArray<FInteractionObjectsRow*> AllRows;
    static const FString ContextString(TEXT("InteractionObjectContext"));
    DropTable->GetAllRows(ContextString, AllRows);

    if (AllRows.IsEmpty()) return nullptr;

    float TotalChance = 0.0f;

    for (const FInteractionObjectsRow* Row : AllRows)
    {
        if (Row)
        {
            TotalChance += Row->SpawnChance;
        }
    }

    const float RandValue = FMath::FRandRange(0.0f, TotalChance);
    float AccumulateChance = 0.0f;

    for (FInteractionObjectsRow* Row : AllRows)
    {
        AccumulateChance += Row->SpawnChance;
        if (RandValue <= AccumulateChance)
        {
            return Row;
        }
    }

    return nullptr;
}

void AMBLGameMode::StartWave()
{
    if (!IsValid(SpawnVolume)) return;

    if (CurrentWave == 0)
    {
        for (int i = 0; i < MaxSpawnObject; ++i)
        {
            if (FInteractionObjectsRow* SelectedRow = GetDropObject())
            {
                if (UClass* ActualClass = SelectedRow->ObjectClass.Get())
                {
                    SpawnVolume->SpawnObject(ActualClass);
                }
            }
        }

        GetWorldTimerManager().SetTimer(
            SpawnTimerHandle,
            this,
            &AMBLGameMode::SpawnManager,
            SpawnInterval,
            true
        );

        CurrentWave++;

        return;
    }

    if (CurrentWave == 1)
    {
        SpawnInterval = 1.0f;
        MaxSpawnEnemy = 20;

        GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
        GetWorldTimerManager().SetTimer(
            SpawnTimerHandle,
            this,
            &AMBLGameMode::SpawnManager,
            SpawnInterval,
            true
        );

        CurrentWave++;

        return;
    }

    if (CurrentWave == 2)
    {
        SpawnInterval = 0.5f;
        MaxSpawnEnemy = 30;

        GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
        GetWorldTimerManager().SetTimer(
            SpawnTimerHandle,
            this,
            &AMBLGameMode::SpawnManager,
            SpawnInterval,
            true
        );

        CurrentWave++;
        
        return;
    }

    if (CurrentWave == MaxWave)
    {
        SpawnBoss();
        UE_LOG(LogTemp, Warning, TEXT("BossSpawn"));
        return;
    }
    
}

void AMBLGameMode::SpawnBoss()
{
    SpawnVolume->SpawnEnemy(Boss);

    GetWorldTimerManager().SetTimer(
        GameOverTimerHandle,
        this,
        &AMBLGameMode::GameOver,
        WaveDuration,
        false
    );

    GetWorldTimerManager().ClearTimer(NextWaveTimerHandle);
}

void AMBLGameMode::SpawnManager()
{
    if (!SpawnVolume) return;

    if (MaxSpawnEnemy > CurrentEnemy)
    {
        SpawnVolume->SpawnEnemy(Enemy);
        CurrentEnemy++;
    }
    
}

void AMBLGameMode::Dead(AActor* DeadActor)
{
    if (!IsValid(DeadActor)) return;
    
    if (AMBLPlayerCharacter* Player = Cast<AMBLPlayerCharacter>(DeadActor))
    {
        GameOver();
        return;
    }

    if (AMBLNonPlayerCharacter* NPC = Cast<AMBLNonPlayerCharacter>(DeadActor))
    {
        if (CurrentEnemy <= 0)
        {
            CurrentEnemy = 0;
            return;
        }

        CurrentEnemy--;
        return;
    }

    if (AMBLBossCharacter* BossNPC = Cast<AMBLBossCharacter>(DeadActor))
    {
        GameOver();
        return;
    }

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