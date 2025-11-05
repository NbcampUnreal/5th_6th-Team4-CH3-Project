#include "IngameUI/KillCounter.h"
#include "Components/TextBlock.h"

void UKillCounter::UpdateKillCounter(int32 KillCount)
{
	if (KillCounterText)
	{
		KillCounterText->SetText(FText::FromString(
			FString::Printf(TEXT("Kills: %d"), KillCount)
		));
	}
}