#include "IngameUI/UIHUD.h"
#include "IngameUI/XPBar.h"
#include "IngameUI/HPBar.h"
#include "IngameUI/Timer.h"
#include "IngameUI/KillCounter.h"
#include "IngameUI/CoinCounter.h"
#include "IngameUI/Wave.h"
#include "Character/MBLPlayerCharacter.h"
#include "Character/InventoryComponent.h"
#include "IngameUI/UIHorizontalItemList.h"

void UUIHUD::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUIHUD::SetPlayer(TWeakObjectPtr<AMBLPlayerCharacter> InPlayer)
{
	Player = InPlayer;

	UpdateItems();
	if (UInventoryComponent* Inventory = Player->GetComponentByClass<UInventoryComponent>())
	{
		Inventory->OnItemChanged.AddDynamic(this, &ThisClass::UpdateItems);
	}
}

void UUIHUD::UpdateItems()
{
	if (Player.IsValid() == false)
		return;

	if (UInventoryComponent* Inventory = Player->GetComponentByClass<UInventoryComponent>())
	{
		if (IsValid(WeaponList) == true)
		{
			WeaponList->SetItems(Inventory->GetCachedItems(EItemType::Weapon));
		}

		if (IsValid(TomesList) == true)
		{
			TomesList->SetItems(Inventory->GetCachedItems(EItemType::Tomes));
		}

		if (IsValid(TomesList) == true)
		{
			MiscList->SetItems(Inventory->GetCachedItems(EItemType::Misc));
		}
	}
}

void UUIHUD::UpdateXP(float CurrentXP, float MaxXP)
{
	if (XPBarWidget)
	{
		XPBarWidget->UpdateXP(CurrentXP, MaxXP);
	}
}

void UUIHUD::UpdateLevel(int32 CurrentLevel)
{
	if (XPBarWidget)
	{
		XPBarWidget->UpdateLevel(CurrentLevel);
	}
}

void UUIHUD::UpdateKillCount(int32 KillCount)
{
	if (KillCounterWidget)
	{
		KillCounterWidget->UpdateKillCounter(KillCount);
	}
}

void UUIHUD::UpdateTimer(float RemainingTime)
{
	if (TimerWidget)
	{
		TimerWidget->UpdateTimer(RemainingTime);
	}
}

void UUIHUD::UpdateCoinCount(int32 CoinCount)
{
	if (CoinCounterWidget)
	{
		CoinCounterWidget->UpdateCoinCount(CoinCount);
	}
}

void UUIHUD::UpdateWave(int32 CurrentWave)
{
	if (WaveWidget)
	{
		WaveWidget->UpdateWaveText(CurrentWave);

	}
}