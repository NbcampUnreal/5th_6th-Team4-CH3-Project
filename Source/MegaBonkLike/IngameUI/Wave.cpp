#include "IngameUI/Wave.h"
#include "Components/TextBlock.h"

void UWave::UpdateWaveText(int32 CurrentWave)
{
	if (WaveText)
	{
		WaveText->SetText(FText::FromString(FString::Printf(TEXT("Wave: %d"), CurrentWave)));
	}
}