#include "MBLChestObject.h"
#include "Gimmick/Spawn/MBLSpawnSubsystem.h"
#include "Character/MBLPlayerCharacter.h"
#include "Character/InventoryComponent.h"
#include "IngameUI/PopupTags.h"
#include "IngameUI/PopupItemAcquire.h"
#include "Player/MBLPlayerController.h"
#include "Gimmick/Objects/InteractionObjects/GoldManagerSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Gimmick/Objects/UI/ChestWidget.h"
#include "Components/WidgetComponent.h"

AMBLChestObject::AMBLChestObject()
{
}

void AMBLChestObject::BeginPlay()
{
	Super::BeginPlay();

	UpdateRequiredGold();

	if (UGoldManagerSubsystem* GoldManager = GetWorld()->GetSubsystem<UGoldManagerSubsystem>())
	{
		if (!GoldManager->OnRequiredGoldUpdated.IsAlreadyBound(this, &AMBLChestObject::UpdateRequiredGold))
		{
			GoldManager->OnRequiredGoldUpdated.AddDynamic(this, &AMBLChestObject::UpdateRequiredGold);
		}
	}
}

void AMBLChestObject::OnObjectActivated(AActor* Activator)
{
	UWorld* World = GetWorld();
	if (!IsValid(World)) return;

	UGoldManagerSubsystem* GoldManager = World->GetSubsystem<UGoldManagerSubsystem>();
	if (!IsValid(GoldManager)) return;
	
	
	AMBLPlayerCharacter* Player = Cast<AMBLPlayerCharacter>(Activator);
	if (!IsValid(Player)) return;

	if (Player->UseGold(GoldManager->GetRequiredGold()))
	{
		UInventoryComponent* Inventory = Player->FindComponentByClass<UInventoryComponent>();
		if (!IsValid(Inventory)) return;

		AMBLPlayerController* PlayerController = Cast<AMBLPlayerController>(Player->GetController());
		if (!IsValid(PlayerController)) return;

		UPopupItemAcquire* PopupItemAcquire = Cast<UPopupItemAcquire>(PlayerController->MakePopup(TAG_Popup_AcquireItem));
		if (!IsValid(PopupItemAcquire)) return;

		GoldManager->NextPhase();
		PopupItemAcquire->SetInventory(Inventory);
		PopupItemAcquire->SetOption();
		Super::DestroyObject();
	}
}

void AMBLChestObject::UpdateRequiredGold()
{
	if (UUserWidget* Widget = InteractableWidget->GetWidget())
	{
		if (UChestWidget* ChestWidget = Cast<UChestWidget>(Widget))
		{
			if (UGoldManagerSubsystem* GoldManager = GetWorld()->GetSubsystem<UGoldManagerSubsystem>())
			{
				ChestWidget->SetGoldText(GoldManager->GetRequiredGold());
			}
		}
	}
}
