#include "IngameUI/Timer.h"
#include "Components/TextBlock.h"

void UTimer::UpdateTimer(float RemainingTime)
{
	if (TimerText)
	{
		int32 TotalSeconds = FMath::CeilToInt(RemainingTime);
		int32 Minutes = TotalSeconds / 60;
		int32 Seconds = TotalSeconds % 60;

		FString TimeString = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
		TimerText->SetText(FText::FromString(TimeString));
	}
}
