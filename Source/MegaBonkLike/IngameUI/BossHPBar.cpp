#include "IngameUI/BossHPBar.h"
#include "Components/ProgressBar.h"

void UBossHPBar::UpdateBossHP(float CurrentHP, float MaxHP)
{
	if (BossHPProgressBar)
	{
		BossHPProgressBar->SetPercent(CurrentHP / MaxHP);
	}
}
