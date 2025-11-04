#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Item/ItemSelectOption.h"
#include "UIItemSelectOption.generated.h"

class UTextBlock;
class UImage;
class UButton;
enum class EItemType : uint8;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemSelected, const FItemSelectOption&, SelectItemOption);

UCLASS()
class MEGABONKLIKE_API UUIItemSelectOption : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual bool Initialize() override;

	void SetOption(const FItemSelectOption& InOption);
	UFUNCTION()
	void OnItemButtonClicked();

	FOnItemSelected OnItemSelected;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ButtonOption;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ImgBackground;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextRarity;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextName;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextDesc;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextLevel;

	UPROPERTY(EditAnywhere)
	TMap<EItemType, TObjectPtr<UDataTable>> ItemTables;
	UPROPERTY()
	FItemSelectOption ItemOption;
};
