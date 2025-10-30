#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MBLGameMode.generated.h"

UCLASS()
class MEGABONKLIKE_API AMBLGameMode : public AGameMode
{
	GENERATED_BODY()


public:
	virtual void BeginPlay() override;

	FTimerHandle SpawnTimerHandle;

	void StartWave();
	void SpawnManager();
	void GameOver();

};
