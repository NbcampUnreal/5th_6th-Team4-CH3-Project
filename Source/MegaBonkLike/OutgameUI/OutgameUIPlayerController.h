
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "OutgameUIPlayerController.generated.h"


UCLASS()
class MEGABONKLIKE_API AOutgameUIPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AOutgameUIPlayerController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Menu")
	UUserWidget* MainMenuWidgetInstance;

	UFUNCTION(BlueprintCallable, Category = "Menu")
	void ShowMainMenu(bool bIsRestart);
	UFUNCTION(BlueprintCallable, Category = "Menu")
	void StartGame();
	UFUNCTION(BlueprintCallable, Category = "Menu")
	void ExitGame();

protected:
	virtual void BeginPlay() override;
};
