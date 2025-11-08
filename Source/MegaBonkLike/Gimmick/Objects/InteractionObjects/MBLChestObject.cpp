#include "MBLChestObject.h"
#include "Gimmick/Spawn/MBLSpawnSubsystem.h"
#include "Character/MBLPlayerCharacter.h"
#include "Character/InventoryComponent.h"
#include "IngameUI/PopupTags.h"
#include "IngameUI/PopupItemAcquire.h"
#include "Player/MBLPlayerController.h"
#include "Gimmick/Objects/InteractionObjects/GoldManagerSubsystem.h"
#include "Kismet/GameplayStatics.h"

AMBLChestObject::AMBLChestObject()
{
}

void AMBLChestObject::OnObjectActivated(AActor* Activator)
{
	UWorld* World = GetWorld();
	if (!IsValid(World)) return;

	UGoldManagerSubsystem* GoldManager = World->GetSubsystem<UGoldManagerSubsystem>();
	if (!IsValid(GoldManager)) return;
	
	
	AMBLPlayerCharacter* Player = Cast<AMBLPlayerCharacter>(Activator);
	if (!IsValid(Player)) return;

	if (Player->UseGold(GoldManager->SearchChestRequiredGold()))
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

	//if (GoldManager->TryOpenChest(Gold))
	//{
	//	UInventoryComponent* Inventory = Player->FindComponentByClass<UInventoryComponent>();
	//	if (!IsValid(Inventory)) return;

	//	AMBLPlayerController* PlayerController = Cast<AMBLPlayerController>(Player->GetController());
	//	if (!IsValid(PlayerController)) return;

	//	UPopupItemAcquire* PopupItemAcquire = Cast<UPopupItemAcquire>(PlayerController->MakePopup(TAG_Popup_AcquireItem));
	//	if (!IsValid(PopupItemAcquire)) return;

	//	PopupItemAcquire->SetInventory(Inventory);
	//	PopupItemAcquire->SetOption();
	//	Super::DestroyObject();
	//}

	//UInventoryComponent* Inventory = Player->FindComponentByClass<UInventoryComponent>();
	//if (!IsValid(Inventory)) return;

	//AMBLPlayerController* PlayerController = Cast<AMBLPlayerController>(Player->GetController());
	//if (!IsValid(PlayerController)) return;

	//UPopupItemAcquire* PopupItemAcquire = Cast<UPopupItemAcquire>(PlayerController->MakePopup(TAG_Popup_AcquireItem));
	//if (!IsValid(PopupItemAcquire)) return;

	//PopupItemAcquire->SetInventory(Inventory);
	//PopupItemAcquire->SetOption();
	//Super::DestroyObject();


	//if (AMBLPlayerCharacter* Player = Cast<AMBLPlayerCharacter>(Activator))
	//{
	//	if (UInventoryComponent* Inventory = Player->FindComponentByClass<UInventoryComponent>())
	//	{
	//		if (AMBLPlayerController* PlayerController = Cast<AMBLPlayerController>(Player->GetController()))
	//		{
	//			if (UPopupItemAcquire* PopupItemAcquire = Cast<UPopupItemAcquire>(PlayerController->MakePopup(TAG_Popup_AcquireItem)))
	//			{
	//				PopupItemAcquire->SetInventory(Inventory);
	//				PopupItemAcquire->SetOption();
	//				Super::DestroyObject();
	//			}
	//		}
	//	}
	//}
}
