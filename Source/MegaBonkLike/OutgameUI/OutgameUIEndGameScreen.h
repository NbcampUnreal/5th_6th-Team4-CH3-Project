#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OutgameUIEndGameScreen.generated.h"

class UUIHorizontalItemList;
class AMBLPlayerCharacter;

UCLASS()
class MEGABONKLIKE_API UOutgameUIEndGameScreen : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	void SetPlayer(TWeakObjectPtr<AMBLPlayerCharacter> InPlayer);

	void SetOutgameUIScore();

	void SetOutgameUIInventory();

private:
	TWeakObjectPtr<AMBLPlayerCharacter> Player;

	UPROPERTY(meta = (BindWidget))
	UUIHorizontalItemList* WeaponList;

	UPROPERTY(meta = (BindWidget))
	UUIHorizontalItemList* TomesList;

	UPROPERTY(meta = (BindWidget))
	UUIHorizontalItemList* MiscList;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* KillsText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TimeText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* LevelText;
};
