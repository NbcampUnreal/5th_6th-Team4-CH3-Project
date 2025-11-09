#include "Gimmick/Objects/UI/InteractionWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UInteractionWidget::SetInteractionText(FName ObjectType)
{
	InteractionText->SetText(FText::FromString(ObjectType.ToString()));
}
