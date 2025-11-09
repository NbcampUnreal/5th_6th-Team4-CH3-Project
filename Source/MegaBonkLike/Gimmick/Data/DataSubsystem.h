#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DataSubsystem.generated.h"

struct FChestRequiredGoldRow;

UCLASS(Blueprintable)
class MEGABONKLIKE_API UDataSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere)
	UDataTable* RequiredGoldTable;

public:
	UDataSubsystem();
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	bool GetChestRequiredGoldRow(int32 RowIndex, FChestRequiredGoldRow& OutRow) const;
	int32 GetLastPhase() const;
};
