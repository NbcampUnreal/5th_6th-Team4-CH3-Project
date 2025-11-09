#include "Gimmick/Objects/UI/ChestWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Engine/World.h"
#include "TimerManager.h"


void UChestWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	HideMessage();
}

void UChestWidget::ViewDenyMessage()
{
	if (IsValid(LockIcon) && IsValid(DenyText))
	{
		LockIcon->SetVisibility(ESlateVisibility::Visible);
		DenyText->SetVisibility(ESlateVisibility::Visible);
	}

	if (IsValid(GetWorld()))
	{
		GetWorld()->GetTimerManager().ClearTimer(MessageTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(
			MessageTimerHandle,
			this,
			&UChestWidget::HideMessage,
			2.0f,
			false
		);
	}
}

void UChestWidget::HideMessage()
{
	if (IsValid(LockIcon))
	{
		LockIcon->SetVisibility(ESlateVisibility::Hidden);
	}

	if (IsValid(DenyText))
	{
		DenyText->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UChestWidget::SetGoldText(float RequiredGold)
{
	if (!IsValid(GoldText)) return;

	GoldText->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), RequiredGold)));
}
