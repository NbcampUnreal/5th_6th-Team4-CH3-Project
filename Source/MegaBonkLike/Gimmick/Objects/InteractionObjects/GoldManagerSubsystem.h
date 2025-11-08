#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Gimmick/Data/ChestRequiredGoldRow.h"
#include "GoldManagerSubsystem.generated.h"

UCLASS(Blueprintable)
class MEGABONKLIKE_API UGoldManagerSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

private:
	int32 Phase;
	FChestRequiredGoldRow SearchRow;

public:
	UGoldManagerSubsystem();
	bool TryOpenChest(float& PossessionGold);
	void NextPhase();
	float SearchChestRequiredGold();

};
