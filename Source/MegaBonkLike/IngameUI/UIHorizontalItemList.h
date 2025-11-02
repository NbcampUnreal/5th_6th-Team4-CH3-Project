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
	void SetItems(const TArray<TWeakObjectPtr<UItemBase>>& Items);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UPanelWidget> ParentPanel;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UUIItemSlot> ItemSlotClass;
};
