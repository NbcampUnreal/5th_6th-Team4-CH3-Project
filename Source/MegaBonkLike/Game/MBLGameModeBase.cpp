#include "Game/MBLGameModeBase.h"
#include "Controller/MBLPlayerController.h"
#include "Character/MBLPlayerCharacter.h"

AMBLGameModeBase::AMBLGameModeBase()
{
	PlayerControllerClass = AMBLPlayerController::StaticClass();
	DefaultPawnClass = AMBLPlayerCharacter::StaticClass();
}
