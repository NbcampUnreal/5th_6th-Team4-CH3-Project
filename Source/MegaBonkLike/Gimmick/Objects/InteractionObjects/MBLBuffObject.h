#pragma once

#include "CoreMinimal.h"
#include "Gimmick/Objects/InteractionObjects/MBLBaseInteractionObject.h"
#include "GameplayTagContainer.h"
#include "Attribute/AttributeModifier.h"
#include "MBLBuffObject.generated.h"

struct FShrineOption;

UCLASS()
class MEGABONKLIKE_API AMBLBuffObject : public AMBLBaseInteractionObject
{
	GENERATED_BODY()
	
public:
	AMBLBuffObject();
	virtual void OnObjectActivated(AActor* Activator) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Buffs", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDataTable> ItemRarityTable;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Buffs", meta = (AllowPrivateAccess = "true"))
	TMap<FGameplayTag, FAttributeModifier> SantuaryOptions;
	const int32 MinTag = 3;
	TArray<FGameplayTag> GetRandomTag() const;
	TArray<FShrineOption> GetRandomOptions();
	const struct FItemRarityDataRow* GetRandomRarity() const;
};
