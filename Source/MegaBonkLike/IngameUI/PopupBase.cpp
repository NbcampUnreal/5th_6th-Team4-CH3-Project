#include "IngameUI/PopupBase.h"
#include "Animation/WidgetAnimation.h"

void UPopupBase::NativeConstruct()
{
	Super::NativeConstruct();
	if (OpenAnimation)
	{
		PlayAnimation(OpenAnimation);
	}
}

void UPopupBase::NativeDestruct()
{
	OnDestroy.Broadcast();
	Super::NativeDestruct();
}
