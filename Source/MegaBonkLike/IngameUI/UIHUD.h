#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIHUD.generated.h"

class UUIHorizontalItemList;
class AMBLPlayerCharacter;
class UXPBar;
class UKillCounter;
class UTimer;
class UCoinCounter;
class UWave;
class UBossHPBar;

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

	UFUNCTION(BlueprintCallable)
	void UpdateLevel(int32 CurrentLevel);

	//킬카운터
	UFUNCTION(BlueprintCallable)
	void UpdateKillCount(int32 KillCount);

	//타이머
	UFUNCTION(BlueprintCallable)
	void UpdateTimer(float RemainingTime);

	//코인카운터
	UFUNCTION(BlueprintCallable)
	void UpdateCoinCount(int32 CoinCount);

	//Wave
	UFUNCTION(BlueprintCallable)
	void UpdateWave(int32 CurrentWave, int32 MaxWave);

	//BossWave
	UFUNCTION(BlueprintCallable)
	void UpdateBossWaveText();

	//BossHP
	UFUNCTION(BlueprintCallable)
	void ShowBossHPBar(bool bshow);

	UFUNCTION(BlueprintCallable)
	void UpdateBossHP(float CurrentHP, float MaxHP);

	UFUNCTION()
	void UpdateItems();

protected:

	//경험치
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UXPBar> XPBarWidget;

	//킬카운터
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UKillCounter> KillCounterWidget;

	//타이머
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTimer> TimerWidget;

	//코인카운터
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCoinCounter> CoinCounterWidget;;

	//Wave
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWave> WaveWidget;

	//Boss
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UBossHPBar> BossHPBarClass;

	UPROPERTY()
	TObjectPtr<UBossHPBar> BossHPBarWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUIHorizontalItemList> WeaponList;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUIHorizontalItemList> TomesList;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUIHorizontalItemList> MiscList;

	TWeakObjectPtr<AMBLPlayerCharacter> Player;
};
