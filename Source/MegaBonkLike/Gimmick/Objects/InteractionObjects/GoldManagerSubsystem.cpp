#include "Gimmick/Objects/InteractionObjects/GoldManagerSubsystem.h"
#include "Gimmick/Data/DataSubsystem.h"
#include "Kismet/GameplayStatics.h"

UGoldManagerSubsystem::UGoldManagerSubsystem()
	: Phase(1)
	, LastPhase(0)
	, LastRequiredGold(0.0f)
	, SearchRow({})
{
}

void UGoldManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	if (UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(this))
	{
		if (UDataSubsystem* Data = GameInstance->GetSubsystem<UDataSubsystem>())
		{
			LastPhase = Data->GetLastPhase();
		}
	}

	SearchCurrentPhaseRequiredGold();
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

	if (LastPhase >= Phase)
	{
		Data->GetChestRequiredGoldRow(Phase, SearchRow);
		LastRequiredGold = SearchRow.RequiredGold;
		//if (Data->GetChestRequiredGoldRow(Phase, SearchRow))
		//{
		//	OnRequiredGoldUpdated.Broadcast();
		//	return;
		//}
	}
	else
	{
		GenerateRequiredGold();
	}

	OnRequiredGoldUpdated.Broadcast();
}

void UGoldManagerSubsystem::GenerateRequiredGold()
{
	//float RequiredGold = LastRequiredGold;
	//int32 ExtraPhase = Phase - LastPhase;
	const float GrowthRate = 1.08f;
	float GeneratedRequiredGold = LastRequiredGold * FMath::Pow(GrowthRate, Phase - LastPhase);

	SearchRow.RequiredGold = GeneratedRequiredGold;
}

