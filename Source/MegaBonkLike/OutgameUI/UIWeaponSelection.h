#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIWeaponSelection.generated.h"

class UUIGridItemList;
class UItemBase;

UCLASS()
class MEGABONKLIKE_API UUIWeaponSelection : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadOnly)
	int32 SelectedWeaponId;

	UPROPERTY(meta = (BindWidget))
	class UUIWeaponInfoPanel* WeaponInfoPanel;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUIGridItemList> UIGridItemList;

protected:
	UFUNCTION()
	void OnWeaponInfoPanel(UItemBase* ClickedItem);
};
