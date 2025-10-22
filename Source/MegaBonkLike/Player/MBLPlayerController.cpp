#include "Player/MBLPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetSystemLibrary.h"

void AMBLPlayerController::BeginPlay()
{
	UEnhancedInputLocalPlayerSubsystem* InputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (IsValid(InputSystem) == true)
	{
		UKismetSystemLibrary::PrintString(this, TEXT("Add IMC"));
		InputSystem->AddMappingContext(IMC_Base, 0);
	}
}
