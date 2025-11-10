#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIItemSlotInfo.generated.h"

class UTextBlock;
class UItemBase;
enum class EItemRarity : uint8;

UCLASS()
class MEGABONKLIKE_API UUIItemSlotInfo : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetInfo(TWeakObjectPtr<const UItemBase> InItem);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextItemName;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextItemDesc;

	UPROPERTY(EditAnywhere)
	TMap<EItemRarity, FLinearColor> ColorTextRarity;
};
