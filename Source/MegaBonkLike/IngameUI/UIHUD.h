#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIHUD.generated.h"

class UUIHorizontalItemList;
class AMBLPlayerCharacter;

UCLASS()
class MEGABONKLIKE_API UUIHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	void SetPlayer(TWeakObjectPtr<AMBLPlayerCharacter> InPlayer);
	
	UFUNCTION()
	void UpdateItems();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUIHorizontalItemList> WeaponList;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUIHorizontalItemList> TomesList;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUIHorizontalItemList> MiscList;

	TWeakObjectPtr<AMBLPlayerCharacter> Player;
};
