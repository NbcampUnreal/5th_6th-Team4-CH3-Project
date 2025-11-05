#include "IngameUI/CoinCounter.h"
#include "Components/TextBlock.h"

void UCoinCounter::UpdateCoinCount(int32 CoinCount)
{
	if (CoinText)
	{
		CoinText->SetText(FText::FromString(FString::Printf(TEXT("Coin: %d"), CoinCount)));
	}
}
