#include "Gimmick/Data/DataSubsystem.h"
#include "Gimmick/Data/ChestRequiredGoldRow.h"

UDataSubsystem::UDataSubsystem()
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
