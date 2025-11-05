#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIHorizontalItemList.generated.h"

class UUIItemSlot;
class UItemBase;

UCLASS()
class MEGABONKLIKE_API UUIHorizontalItemList : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetItems(const TArray<TWeakObjectPtr<UItemBase>>& Items, int32 DefaultSlotCount = 0);
	void SetHorizontalAlignment(EHorizontalAlignment Alignment);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UWrapBox> ParentPanel;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UUIItemSlot> ItemSlotClass;
};
