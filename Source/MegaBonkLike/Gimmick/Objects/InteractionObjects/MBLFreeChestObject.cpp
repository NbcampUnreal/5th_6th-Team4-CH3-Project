#include "Gimmick/Objects/InteractionObjects/MBLFreeChestObject.h"
#include "Character/MBLPlayerCharacter.h"
#include "Character/InventoryComponent.h"
#include "IngameUI/PopupTags.h"
#include "IngameUI/PopupItemAcquire.h"
#include "Player/MBLPlayerController.h"
#include "Gimmick/Objects/UI/ChestWidget.h"
#include "Components/WidgetComponent.h"

AMBLFreeChestObject::AMBLFreeChestObject()
{
	InteractionObjectType = "FreeChest";
}

void AMBLFreeChestObject::BeginPlay()
{
	Super::BeginPlay();

	UpdateRequiredGold();
}

void AMBLFreeChestObject::OnObjectActivated(AActor* Activator)
{
	UWorld* World = GetWorld();
	if (!IsValid(World)) return;

	AMBLPlayerCharacter* Player = Cast<AMBLPlayerCharacter>(Activator);
	if (!IsValid(Player)) return;

	UInventoryComponent* Inventory = Player->FindComponentByClass<UInventoryComponent>();
	if (!IsValid(Inventory)) return;

	AMBLPlayerController* PlayerController = Cast<AMBLPlayerController>(Player->GetController());
	if (!IsValid(PlayerController)) return;

	UPopupItemAcquire* PopupItemAcquire = Cast<UPopupItemAcquire>(PlayerController->MakePopup(TAG_Popup_AcquireItem));
	if (!IsValid(PopupItemAcquire)) return;

	PlayOpenSound();
	PopupItemAcquire->SetInventory(Inventory);
	PopupItemAcquire->SetOption();
	Super::DestroyObject();

	return;

}

void AMBLFreeChestObject::UpdateRequiredGold()
{
	if (UUserWidget* Widget = InteractableWidget->GetWidget())
	{
		if (UChestWidget* ChestWidget = Cast<UChestWidget>(Widget))
		{
			ChestWidget->SetGoldText(0.f);
		}
	}
}

void AMBLFreeChestObject::UpdateFreeText()
{
	if (UUserWidget* Widget = InteractableWidget->GetWidget())
	{
		if (UChestWidget* ChestWidget = Cast<UChestWidget>(Widget))
		{
			ChestWidget->SetGoldText(0.f);
		}
	}
}
