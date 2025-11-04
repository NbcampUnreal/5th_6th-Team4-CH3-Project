#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "MBLPlayerController.generated.h"

class UInputMappingContext;
//class UXPBar;
class UUIHUD;

UCLASS()
class MEGABONKLIKE_API AMBLPlayerController : public APlayerController
{
	GENERATED_BODY()


protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<UUIHUD> HUDWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HUD")
	TObjectPtr<UUIHUD> HUDWidgetInstance;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Menu")
	UUserWidget* MainMenuWidgetInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu")
	TSubclassOf<UUserWidget> PauseMenuClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu")
	TSubclassOf<UUserWidget> EndGameScreenWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Menu")
	UUserWidget* EndGameScreenWidgetInstance;

	UFUNCTION(BlueprintPure, Category = "HUD")
	UUserWidget* GetHUDWidget() const;

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowGameHUD();

	UFUNCTION(BlueprintCallable, Category = "HUD|XPBar")
	void UpdateXP(float CurrentXP, float MaxXP);

	UFUNCTION(BlueprintCallable, Category = "HUD|XPBar")
	void UpdatePlayerLevel(int32 NewLevel);

	UFUNCTION(BlueprintCallable, Category = "HUD|KillCounter")
	void UpdateKillCount(int32 KillCount);

	UFUNCTION(BlueprintCallable, Category = "HUD|Timer")
	void UpdateTimer(float RemainingTime);

	UFUNCTION(BlueprintCallable, Category = "HUD|CoinCounter")
	void UpdateCoinCount(int32 CoinCount);
	
	UFUNCTION(BlueprintCallable, Category = "HUD|Wave")
	void UpdateWave(int32 CurrentWave);

	UFUNCTION(BlueprintCallable, Category = "Menu")
	void ShowMainMenu(bool bIsRestart);

	UFUNCTION(BlueprintCallable, Category = "Menu")
	void ShowEndGameScreen(bool bIsRestart);

	UFUNCTION(BlueprintCallable, Category = "Menu")
	void RestartGame();
	UFUNCTION(BlueprintCallable, Category = "Menu")
	void QuitGame();

	UPROPERTY()
	UUserWidget* PauseMenuInstance;

	UFUNCTION(BlueprintCallable, Category = "Menu")
	void TogglePauseMenu();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> IMC_Base;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Pause;

};


