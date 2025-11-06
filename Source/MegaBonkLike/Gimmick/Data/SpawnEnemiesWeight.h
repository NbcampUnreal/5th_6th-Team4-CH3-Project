#pragma once

#include "CoreMinimal.h"
#include "Character/EnemyBase.h"
#include "Engine/DataTable.h"
#include "SpawnEnemiesWeight.generated.h"

USTRUCT(BlueprintType)
struct MEGABONKLIKE_API FSpawnEnemiesWeight
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AEnemyBase> Enemy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Weight;
};

USTRUCT(BlueprintType)
struct MEGABONKLIKE_API FSpawnEnemyList : public FTableRowBase
{
	GENERATED_BODY()
	// RowName = EMBLWaveState::Wave...
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSpawnEnemiesWeight> Enemies;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnInterval;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 MaxSpawnEnemy;
};