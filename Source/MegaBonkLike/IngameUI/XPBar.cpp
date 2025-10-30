#include "IngameUI/XPBar.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"


void UXPBar::UpdateXP(float CurrentXP, float MaxXP)
{
	if (XPProgressBar)
	{
		XPProgressBar->SetPercent(MaxXP > 0 ? CurrentXP / MaxXP : 0.0f);
	}

	if (XPText)
	{
		XPText->SetText(FText::FromString(
			FString::Printf(TEXT("%.0f / %.0f"), CurrentXP, MaxXP)
		));
	}
}