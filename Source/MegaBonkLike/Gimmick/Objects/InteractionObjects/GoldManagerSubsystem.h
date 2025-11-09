#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Gimmick/Data/ChestRequiredGoldRow.h"
#include "GoldManagerSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRequiredGoldUpdated);

UCLASS(Blueprintable)
class MEGABONKLIKE_API UGoldManagerSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

private:
	int32 Phase;
	FChestRequiredGoldRow SearchRow;

public:
	UGoldManagerSubsystem();

	UPROPERTY()
	FOnRequiredGoldUpdated OnRequiredGoldUpdated;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	void NextPhase();
	float GetRequiredGold();
	void SearchCurrentPhaseRequiredGold();

};
