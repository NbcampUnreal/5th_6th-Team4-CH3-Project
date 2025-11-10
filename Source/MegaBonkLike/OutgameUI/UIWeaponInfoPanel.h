#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIWeaponInfoPanel.generated.h"

class UWidgetSwitcher;
class UTextBlock;
class UItemBase;
struct FItemDataRow;

UCLASS()
class MEGABONKLIKE_API UUIWeaponInfoPanel : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "WeaponInfo")
    void SetWeaponInfo(UItemBase* WeaponItem);

protected:
    UPROPERTY(meta = (BindWidget))
    UWidgetSwitcher* WeaponInfoSwitcher;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> WeaponName;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> WeaponDesc;
};
