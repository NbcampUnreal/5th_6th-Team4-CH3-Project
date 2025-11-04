#include "IngameUI/PopupBase.h"

void UPopupBase::NativeDestruct()
{
	OnDestroy.Broadcast();
	Super::NativeDestruct();
}
