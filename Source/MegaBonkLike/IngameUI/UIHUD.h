#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIHUD.generated.h"

class UUIHorizontalItemList;
class AMBLPlayerCharacter;
class UXPBar;

UCLASS()
class MEGABONKLIKE_API UUIHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	void SetPlayer(TWeakObjectPtr<AMBLPlayerCharacter> InPlayer);
	
	//경험치
	UFUNCTION(BlueprintCallable)
	void UpdateXP(float CurrentXP, float MaxXP);

	UFUNCTION()
	void UpdateItems();

protected:

	//경험치
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UXPBar> XPBarWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUIHorizontalItemList> WeaponList;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUIHorizontalItemList> TomesList;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUIHorizontalItemList> MiscList;

	TWeakObjectPtr<AMBLPlayerCharacter> Player;
};
