#include "IngameUI/HPBar.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UHPBar::UpdateHP(float CurrentHP, float MaxHP)
{
    if (HPProgressBar)
    {
        HPProgressBar->SetPercent(CurrentHP / MaxHP);
    }

    if (HPText)
    {
        HPText->SetText(FText::FromString(
            FString::Printf(TEXT("%.0f / %.0f"), CurrentHP, MaxHP)
        ));
    }
}