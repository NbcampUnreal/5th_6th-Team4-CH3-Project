#include "Player/MBLPlayerController.h"
#include "EnhancedInputSubsystems.h"

void AMBLPlayerController::BeginPlay()
{
	UEnhancedInputLocalPlayerSubsystem* InputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (IsValid(InputSystem) == true)
	{
		InputSystem->AddMappingContext(IMC_Base, 0);
	}
}
