#pragma once

#include "CoreMinimal.h"
#include "MBLWaveEnums.generated.h"

UENUM()
enum class EMBLWaveState : uint8
{
	Wave1,
	Wave2,
	Wave3,
	FinalWave,
	Finished
};