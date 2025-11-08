#include "Gimmick/Objects/InteractionObjects/GoldManagerSubsystem.h"
#include "Gimmick/Data/DataSubsystem.h"
#include "Kismet/GameplayStatics.h"

UGoldManagerSubsystem::UGoldManagerSubsystem()
	: Phase(1)
	, SearchRow({})
{
}

bool UGoldManagerSubsystem::TryOpenChest(float& PossessionGold)
{
	float RequiredGold = SearchChestRequiredGold();
	if (FMath::IsNearlyZero(RequiredGold)) return false;

	if (PossessionGold >= RequiredGold)
	{
		PossessionGold -= RequiredGold;
		Phase++;
		return true;
	}

	return false;
}

void UGoldManagerSubsystem::NextPhase()
{
	Phase++;
}

float UGoldManagerSubsystem::SearchChestRequiredGold()
{
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(this);
	if (!IsValid(GameInstance)) return 0.0f;

	UDataSubsystem* Data = GameInstance->GetSubsystem<UDataSubsystem>();
	if (!IsValid(Data)) return 0.0f;

	if (Data->GetChestRequiredGoldRow(Phase, SearchRow))
	{
		return SearchRow.RequiredGold;
	}

	return 0.0f;
}

