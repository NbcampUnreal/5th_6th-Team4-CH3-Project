#include "Game/MBLGameMode.h"
#include "Game/MBLGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Character/MBLPlayerCharacter.h"
#include "Character/MBLNonPlayerCharacter.h"
#include "Character/MBLBossCharacter.h"
#include "Player/MBLPlayerController.h"
#include "Gimmick/Spawn/MBLSpawnVolume.h"
#include "Gimmick/Data/InteractionObjectsRow.h"
#include "Gimmick/Data/SpawnEnemiesWeight.h"

AMBLGameMode::AMBLGameMode()
    : SpawnVolume(nullptr)
    , Enemy()
    , EnemyTable(nullptr)
    , Boss(nullptr)
    , DropTable(nullptr)
    , CurrentWave(EMBLWaveState::SetWave)
    , WaveDuration(20.0f)
    , MaxSpawnObject(500)
    , SpawnInterval(2.0f)
    , MaxSpawnEnemy(10)
    , CurrentEnemy(0)
{
    DefaultPawnClass = AMBLPlayerCharacter::StaticClass();
    PlayerControllerClass = AMBLPlayerController::StaticClass();
    GameStateClass = AMBLGameState::StaticClass(); 
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

void AMBLGameMode::SpawnManager()
{
    if (!IsValid(SpawnVolume)) return;

    if (MaxSpawnEnemy > CurrentEnemy)
    {
        AEnemyBase* NewEnemy = SpawnVolume->SpawnEnemy(GetEnemyClass(CurrentWave));
        if (!IsValid(NewEnemy)) return;

        NewEnemy->SetAttack(CurrentWave);
        NewEnemy->SetSpeed(CurrentWave);
        NewEnemy->SetColor(CurrentWave);

        CurrentEnemy++;
    }
}

void AMBLGameMode::SpawnBoss()
{
    AEnemyBase* NewEnemy = SpawnVolume->SpawnEnemy(Boss);
    if (!IsValid(NewEnemy)) return;

    GetWorldTimerManager().SetTimer(
        GameOverTimerHandle,
        this,
        &AMBLGameMode::GameOver,
        WaveDuration,
        false
    );
}

FInteractionObjectsRow* AMBLGameMode::GetDropObject() const
{
    if (!IsValid(DropTable)) return nullptr;

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

TSubclassOf<AEnemyBase> AMBLGameMode::GetEnemyClass(EMBLWaveState Wave) const
{
    if (!IsValid(EnemyTable)) return nullptr;

    if (Wave == EMBLWaveState::SetWave) return nullptr;

    if (Wave >= EMBLWaveState::Wave3) Wave = EMBLWaveState::Wave3;

    FName RowName(*StaticEnum<EMBLWaveState>()->GetNameStringByValue((int64)Wave));
    FSpawnEnemyList* EnemyInfo = EnemyTable->FindRow<FSpawnEnemyList>(RowName, TEXT(""));

    if (!EnemyInfo) return nullptr;

    const TArray<FSpawnEnemiesWeight>& EnemyList = EnemyInfo->Enemies;

    float TotalWeight = 0.0f;

    for (const auto& Info : EnemyList)
    {
        TotalWeight += Info.Weight;
    }

    float RandValue = FMath::FRandRange(0.0f, TotalWeight);
    float Drawing = 0.0f;

    for (const auto& Info : EnemyList)
    {
        Drawing += Info.Weight;
        if (RandValue <= Drawing)
        {
            return Info.Enemy;
        }
    }

    return nullptr;
}

void AMBLGameMode::DeadPlayer()
{
    GameOver();
}

void AMBLGameMode::DeadEnemy()
{
    if (CurrentEnemy <= 0)
    {
        CurrentEnemy = 0;
        return;
    }

    CurrentEnemy--;

    if (AMBLGameState* GS = GetGameState<AMBLGameState>())  //추가
    {
        GS->Addkill();
    }
}

void AMBLGameMode::DeadBoss()
{
    if (AMBLGameState* GS = GetGameState<AMBLGameState>())  //추가
    {
        GS->Addkill();
    }

    GameOver();
}

float AMBLGameMode::GetWaveDuration() const
{
    return WaveDuration;
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

void AMBLGameMode::StartWave()
{
    if (!IsValid(SpawnVolume)) return;

    if (CurrentWave == EMBLWaveState::SetWave)
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

        //IngameUI
        if (AMBLGameState* CurrentGameState = GetGameState<AMBLGameState>())
        {
            CurrentGameState->StartWave();
        }
        //IngameUI

        NextWave(CurrentWave);
        return;
    }

    NextWave(CurrentWave);
}

void AMBLGameMode::NextWave(EMBLWaveState& Wave)
{
    UE_LOG(LogTemp, Warning, TEXT("Wave %d"), Wave);
    if (Wave == EMBLWaveState::Finished) return;

    GetWorldTimerManager().ClearTimer(SpawnTimerHandle);

    if (Wave == EMBLWaveState::SetWave)
    {
        Wave = GetNextWave(Wave);

        FName RowName(*StaticEnum<EMBLWaveState>()->GetNameStringByValue((int64)Wave));
        FSpawnEnemyList* EnemyInfo = EnemyTable->FindRow<FSpawnEnemyList>(RowName, TEXT(""));

        if (!EnemyInfo) return;

        SpawnInterval = EnemyInfo->SpawnInterval;
        MaxSpawnEnemy = EnemyInfo->MaxSpawnEnemy;

        GetWorldTimerManager().SetTimer(
            SpawnTimerHandle,
            this,
            &AMBLGameMode::SpawnManager,
            SpawnInterval,
            true
        );

        return;
    }

    FName RowName(*StaticEnum<EMBLWaveState>()->GetNameStringByValue((int64)Wave));
    FSpawnEnemyList* EnemyInfo = EnemyTable->FindRow<FSpawnEnemyList>(RowName, TEXT(""));

    if (!EnemyInfo) return;

    SpawnInterval = EnemyInfo->SpawnInterval;
    MaxSpawnEnemy = EnemyInfo->MaxSpawnEnemy;

    /*if (Wave != EMBLWaveState::SetWave)
    {
        FName RowName(*StaticEnum<EMBLWaveState>()->GetNameStringByValue((int64)Wave));
        FSpawnEnemyList* EnemyInfo = EnemyTable->FindRow<FSpawnEnemyList>(RowName, TEXT(""));

        if (!EnemyInfo) return;

        SpawnInterval = EnemyInfo->SpawnInterval;
        MaxSpawnEnemy = EnemyInfo->MaxSpawnEnemy;
    }*/

    GetWorldTimerManager().SetTimer(
        SpawnTimerHandle,
        this,
        &AMBLGameMode::SpawnManager,
        SpawnInterval,
        true
    );

    if (Wave == EMBLWaveState::FinalWave) SpawnBoss();

    Wave = GetNextWave(Wave);
}
