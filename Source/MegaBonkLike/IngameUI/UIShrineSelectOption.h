#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Gimmick/Data/ShrineOption.h"
#include "UIShrineSelectOption.generated.h"

class UTextBlock;
class UImage;
class UButton;
class UBorder;
enum class EItemRarity : uint8;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSelected, const FShrineOption&, Option);

UCLASS()
class MEGABONKLIKE_API UUIShrineSelectOption : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;

	void SetOption(const FShrineOption& InOption);

	UFUNCTION()
	void OnButtonClicked();

	FOnSelected OnSelected;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ButtonOption;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ImgBackground;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextRarity;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextOptionName;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextOptionValue;
	UPROPERTY()
	TArray<TObjectPtr<UBorder>> Borders;

	UPROPERTY(EditAnywhere)
	TMap<EItemRarity, FLinearColor> ColorImgBackground;
	UPROPERTY(EditAnywhere)
	TMap<EItemRarity, FLinearColor> ColorBorder;

	UPROPERTY()
	FShrineOption Option;

	static const FName BorderNames[4];
};
