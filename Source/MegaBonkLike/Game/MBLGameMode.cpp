#include "Game/MBLGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Character/MBLPlayerCharacter.h"
#include "Player/MBLPlayerController.h"
#include "Gimmick/Spawn/MBLSpawnVolume.h"
#include "Gimmick/Data/InteractionObjectsRow.h"
#include "Character/MBLBossCharacter.h"

AMBLGameMode::AMBLGameMode()
    : SpawnVolume(nullptr)
    , MaxSpawnObject(50)
    , Enemy(nullptr)
    , Boss(nullptr)
    , SpawnInterval(0.5f)
    , MaxSpawnEnemy(40)
    , BossTime(10)
    , GameTime(30.f)
    , CurrentEnemy(0)
    , DropTable(nullptr)
{
    DefaultPawnClass = AMBLPlayerCharacter::StaticClass();
    PlayerControllerClass = AMBLPlayerController::StaticClass();
}

void AMBLGameMode::BeginPlay()
{
    Super::BeginPlay();

    TArray<AActor*> FoundVolumes;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMBLSpawnVolume::StaticClass(), FoundVolumes);

    SpawnVolume = Cast<AMBLSpawnVolume>(UGameplayStatics::GetActorOfClass(GetWorld(), AMBLSpawnVolume::StaticClass()));

    if (!SpawnVolume) UE_LOG(LogTemp, Error, TEXT("SpawnVolume not found"));

    GetWorldTimerManager().SetTimer(
        GameOverTimerHandle,
        this,
        &AMBLGameMode::GameOver,
        GameTime,
        false
    );

    GetWorldTimerManager().SetTimer(
        BossSpawnTimerHandle,
        this,
        &AMBLGameMode::SpawnBoss,
        BossTime,
        false
    );

    GetWorldTimerManager().SetTimer(
        SpawnTimerHandle,
        this,
        &AMBLGameMode::SpawnManager,
        SpawnInterval,
        true
    );

    StartWave();

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
}

void AMBLGameMode::SpawnBoss()
{
    SpawnVolume->SpawnEnemy(Boss);
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

    if (DeadActor->ActorHasTag("Player"))
    {
        GameOver();
        return;
    }

    if (DeadActor->ActorHasTag("Enemy"))
    {
        if (CurrentEnemy <= 0)
        {
            CurrentEnemy = 0;
            return;
        }

        CurrentEnemy--;
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