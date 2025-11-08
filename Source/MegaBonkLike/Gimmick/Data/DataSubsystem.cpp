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
	if (!LoadedChestRequiredGoldTable) return false;

	static const FString Context = TEXT("Try GetChestRequiredGoldRow");
	FChestRequiredGoldRow* Row = LoadedChestRequiredGoldTable->FindRow<FChestRequiredGoldRow>(*FString::FromInt(RowIndex), Context);
	
	TArray<FName> RowNames = LoadedChestRequiredGoldTable->GetRowNames();
	for (const FName& N : RowNames)
	{
		UE_LOG(LogTemp, Log, TEXT("DataTable RowName: %s"), *N.ToString());
	}

	if (!Row) return false;

	OutRow = *Row;

	return true;
}
