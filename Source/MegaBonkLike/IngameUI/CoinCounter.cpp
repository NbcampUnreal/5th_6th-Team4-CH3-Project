#include "IngameUI/CoinCounter.h"
#include "Components/TextBlock.h"

void UCoinCounter::UpdateCoinCount(int32 CoinCount)
{
	if (CoinCountText)
	{
		CoinCountText->SetText(FText::AsNumber(CoinCount));
	}
}
