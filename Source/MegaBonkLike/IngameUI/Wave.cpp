#include "IngameUI/Wave.h"
#include "Components/TextBlock.h"

void UWave::UpdateWaveText(int32 CurrentWave, int32 MaxWave)
{
	if (WaveText)
	{
		WaveText->SetText(FText::FromString(
			FString::Printf(TEXT("Wave: %d / %d"), CurrentWave, MaxWave)));
	}
}

void UWave::UpdateBossWaveText()
{
	if (WaveText)
	{
		WaveText->SetText(FText::FromString(TEXT("BossWave!")));
	}
}