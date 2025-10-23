#pragma once

#include "CoreMinimal.h"
#include "Character/MBLCharacterBase.h"
#include "MBLNonPlayerCharacter.generated.h"

UCLASS()
class MEGABONKLIKE_API AMBLNonPlayerCharacter : public AMBLCharacterBase
{
	GENERATED_BODY()
	
	AMBLNonPlayerCharacter();

	virtual void BeginPlay() override;
};
