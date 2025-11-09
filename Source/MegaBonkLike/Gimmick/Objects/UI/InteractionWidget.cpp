#include "Gimmick/Objects/UI/InteractionWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UInteractionWidget::SetGoldText(float RequiredGold)
{
	if (!IsValid(GoldText)) return;

	GoldText->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), RequiredGold)));
}
