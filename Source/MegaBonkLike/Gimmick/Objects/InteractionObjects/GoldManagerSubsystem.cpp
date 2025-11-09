#include "Gimmick/Objects/InteractionObjects/GoldManagerSubsystem.h"
#include "Gimmick/Data/DataSubsystem.h"
#include "Kismet/GameplayStatics.h"

UGoldManagerSubsystem::UGoldManagerSubsystem()
	: Phase(1)
	, SearchRow({})
{
}

void UGoldManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UGoldManagerSubsystem::NextPhase()
{
	Phase++;
	SearchCurrentPhaseRequiredGold();
}

float UGoldManagerSubsystem::GetRequiredGold()
{
	return SearchRow.RequiredGold;
}

void UGoldManagerSubsystem::SearchCurrentPhaseRequiredGold()
{
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(this);
	if (!IsValid(GameInstance)) return;

	UDataSubsystem* Data = GameInstance->GetSubsystem<UDataSubsystem>();
	if (!IsValid(Data)) return;

	if (Data->GetChestRequiredGoldRow(Phase, SearchRow))
	{
		OnRequiredGoldUpdated.Broadcast();
		return;
	}
}

