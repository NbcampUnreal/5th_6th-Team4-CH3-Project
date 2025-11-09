#include "Gimmick/Data/DataSubsystem.h"
#include "Gimmick/Data/ChestRequiredGoldRow.h"

UDataSubsystem::UDataSubsystem()
	: RequiredGoldTable(nullptr)
{
}

bool UDataSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return this->GetClass()->IsInBlueprint() && Super::ShouldCreateSubsystem(Outer);
}

void UDataSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

bool UDataSubsystem::GetChestRequiredGoldRow(int32 RowIndex, FChestRequiredGoldRow& OutRow) const
{
	if (!RequiredGoldTable) return false;

	static const FString Context = TEXT("Try GetChestRequiredGoldRow");
	FChestRequiredGoldRow* Row = RequiredGoldTable->FindRow<FChestRequiredGoldRow>(*FString::FromInt(RowIndex), Context);
	
	TArray<FName> RowNames = RequiredGoldTable->GetRowNames();

	if (!Row) return false;

	OutRow = *Row;

	return true;
}

int32 UDataSubsystem::GetLastPhase() const
{
	if (!RequiredGoldTable) return -1;

	const TMap<FName, uint8*>& RowMap = RequiredGoldTable->GetRowMap();
	if (RowMap.IsEmpty()) return -1;

	int32 LastPhase = -1;

	for (auto& Row : RowMap)
	{
		int32 Phase = FCString::Atoi(*Row.Key.ToString());
		LastPhase = FMath::Max(LastPhase, Phase);
	}

	return LastPhase;
}
