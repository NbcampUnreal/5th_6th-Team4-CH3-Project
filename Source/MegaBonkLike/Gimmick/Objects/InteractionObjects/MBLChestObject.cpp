#include "MBLChestObject.h"
#include "Gimmick/Spawn/MBLSpawnSubsystem.h"
#include "Character/MBLPlayerCharacter.h"
#include "Character/InventoryComponent.h"
#include "IngameUI/PopupTags.h"
#include "IngameUI/PopupItemAcquire.h"
#include "Player/MBLPlayerController.h"

AMBLChestObject::AMBLChestObject()
{
}

void AMBLChestObject::OnObjectActivated(AActor* Activator)
{
	if (AMBLPlayerCharacter* Player = Cast<AMBLPlayerCharacter>(Activator))
	{
		if (UInventoryComponent* Inventory = Player->FindComponentByClass<UInventoryComponent>())
		{
			if (AMBLPlayerController* PlayerController = Cast<AMBLPlayerController>(Player->GetController()))
			{
				if (UPopupItemAcquire* PopupItemAcquire = Cast<UPopupItemAcquire>(PlayerController->MakePopup(TAG_Popup_AcquireItem)))
				{
					PopupItemAcquire->SetInventory(Inventory);
					PopupItemAcquire->SetOption();
					Super::DestroyObject();
				}
			}
		}		
	}
}
