#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PopupBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDestroy);

UCLASS()
class MEGABONKLIKE_API UPopupBase : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeDestruct() override;
	
public:
	FOnDestroy OnDestroy;
};
