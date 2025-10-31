#include "Player/MBLPlayerController.h"
#include "IngameUI/UIHUD.h"
#include "Character/MBLPlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Game/MBLGameInstance.h"
#include "Components/TextBlock.h"

void AMBLPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = 
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
		{
			if (IMC_Base)
			{
				InputSystem->AddMappingContext(IMC_Base, 0);
			}
		}
	}
	
	ShowGameHUD();
}

void AMBLPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (IA_Pause)
		{
			EnhancedInput->BindAction(IA_Pause, ETriggerEvent::Started, this, &AMBLPlayerController::TogglePauseMenu);
		}
	}
}

void AMBLPlayerController::ShowGameHUD()
{
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}

	if (HUDWidgetClass)
	{
		HUDWidgetInstance = CreateWidget<UUIHUD>(this, HUDWidgetClass);
		if (HUDWidgetInstance)
		{
			HUDWidgetInstance->AddToViewport();

			bShowMouseCursor = false;
			SetInputMode(FInputModeGameOnly());

			if (AMBLPlayerCharacter* PlayerCharacter = Cast<AMBLPlayerCharacter>(GetPawn()))
			{
				HUDWidgetInstance->SetPlayer(PlayerCharacter);
			}
		}
	}
}

void AMBLPlayerController::UpdateXP(float CurrentXP, float MaxXP)
{
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->UpdateXP(CurrentXP, MaxXP);
	}
}

void AMBLPlayerController::TogglePauseMenu()
{
	if (IsPaused())
	{
		//재개
		SetPause(false);
		if (PauseMenuInstance)
		{
			PauseMenuInstance->RemoveFromParent();
			PauseMenuInstance = nullptr;
		}

		SetInputMode(FInputModeGameOnly());
		bShowMouseCursor = false;
	}
	else
	{
		//일시정지
		SetPause(true);
		if (PauseMenuClass)
		{
			PauseMenuInstance = CreateWidget<UUserWidget>(this, PauseMenuClass);
			if (PauseMenuInstance)
			{
				PauseMenuInstance->AddToViewport();
			}
		}

		SetInputMode(FInputModeUIOnly());
		bShowMouseCursor = true;
	}
}

void AMBLPlayerController::QuitGame()
{
	UKismetSystemLibrary::QuitGame(
		GetWorld(),
		this,
		EQuitPreference::Quit,
		false
	);
}

UUserWidget* AMBLPlayerController::GetHUDWidget() const
{
	return HUDWidgetInstance;
}

void AMBLPlayerController::ShowMainMenu(bool bIsRestart)
{
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}

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

		if (UTextBlock* ButtonText = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName(TEXT("StartButtonText"))))
		{
			if (bIsRestart)
			{
				ButtonText->SetText(FText::FromString(TEXT("Restart")));

			}
			else
			{
				ButtonText->SetText(FText::FromString(TEXT("Start")));
			}
		}

		if (bIsRestart)
		{
			UFunction* PlayAnimFunc = MainMenuWidgetInstance->FindFunction(FName("PlayGameOverAnim"));
			if (PlayAnimFunc)
			{
				MainMenuWidgetInstance->ProcessEvent(PlayAnimFunc, nullptr);
			}

			if (UTextBlock* TotalScoreText = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName("TotalScoreText")))
			{
				if (UMBLGameInstance* MBLGameInstance = Cast<UMBLGameInstance>(UGameplayStatics::GetGameInstance(this)))
				{
					TotalScoreText->SetVisibility(ESlateVisibility::Visible);
					TotalScoreText->SetText(FText::FromString(
						FString::Printf(TEXT("Total Score: "))
					));

				}

			}

		}

	}
}

void AMBLPlayerController::ShowEndGameScreen(bool bIsRestart)
{
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveFromParent();
		HUDWidgetInstance = nullptr;
	}

	if (EndGameScreenWidgetInstance)
	{
		EndGameScreenWidgetInstance->RemoveFromParent();
		EndGameScreenWidgetInstance = nullptr;
	}

	if (EndGameScreenWidgetClass)
	{
		EndGameScreenWidgetInstance = CreateWidget<UUserWidget>(this, EndGameScreenWidgetClass);
		if (EndGameScreenWidgetInstance)
		{
			EndGameScreenWidgetInstance->AddToViewport();

			bShowMouseCursor = true;
			SetInputMode(FInputModeUIOnly());
		}
	}
}

void AMBLPlayerController::RestartGame()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("StartLevel"));
	SetPause(false);
}