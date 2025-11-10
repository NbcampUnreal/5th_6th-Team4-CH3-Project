#include "IngameUI/KillCounter.h"
#include "Components/TextBlock.h"

void UKillCounter::UpdateKillCounter(int32 KillCount)
{
	if (KillCounterText)
	{
		KillCounterText->SetText(FText::AsNumber(KillCount));
	}
}