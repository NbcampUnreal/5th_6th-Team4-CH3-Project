
#include "OutgameUI/OutgameUIPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

AOutgameUIPlayerController::AOutgameUIPlayerController()
	:MainMenuWidgetClass(nullptr),
	MainMenuWidgetInstance(nullptr)
{

}

void AOutgameUIPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FString CurrentMapName = GetWorld()->GetMapName();
	if (CurrentMapName.Contains("StartLevel"))
	{
		ShowMainMenu(false);
	}
}

void AOutgameUIPlayerController::ShowMainMenu(bool bIsRestart)
{
	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetInstance = nullptr;
	}

	if (MainMenuWidgetClass)
	{
		MainMenuWidgetInstance = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);
		if (MainMenuWidgetInstance)
		{
			MainMenuWidgetInstance->AddToViewport();

			bShowMouseCursor = true;
			SetInputMode(FInputModeUIOnly());
		}
	}
}

void AOutgameUIPlayerController::StartGame()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("MainLevel"));
	SetPause(false);
}

void AOutgameUIPlayerController::ExitGame()
{
	UKismetSystemLibrary::QuitGame(this, this, EQuitPreference::Quit, true);
}