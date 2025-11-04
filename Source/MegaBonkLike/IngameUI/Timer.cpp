#include "IngameUI/Timer.h"
#include "Components/TextBlock.h"

void UTimer::UpdateTimer(float RemainingTime)
{
	if (TimerText)
	{
		int32 Second = FMath::CeilToInt(RemainingTime);
		TimerText->SetText(FText::FromString(FString::Printf(TEXT("%02d"), Second)));

	}
}
