#include "IngameUI/UIHUD.h"
#include "IngameUI/XPBar.h"
#include "IngameUI/HPBar.h"
#include "IngameUI/Timer.h"
#include "IngameUI/KillCounter.h"
#include "IngameUI/CoinCounter.h"
#include "IngameUI/Wave.h"
#include "IngameUI/BossHPBar.h"
#include "Character/MBLPlayerCharacter.h"
#include "Character/InventoryComponent.h"
#include "IngameUI/UIHorizontalItemList.h"
#include "Item/ItemEnums.h"

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
			WeaponList->SetItems(Inventory->GetCachedItems(EItemType::Weapon), Inventory->GetMaxWeaponCount());
		}

		if (IsValid(TomesList) == true)
		{
			TomesList->SetItems(Inventory->GetCachedItems(EItemType::Tomes), Inventory->GetMaxTomesCount());
		}

		if (IsValid(TomesList) == true)
		{
			MiscList->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
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

void UUIHUD::UpdateWave(int32 CurrentWave, int32 MaxWave)
{
	if (WaveWidget)
	{
		WaveWidget->UpdateWaveText(CurrentWave, MaxWave);
	}
}

void UUIHUD::UpdateBossWaveText()
{
	if (WaveWidget)
	{
		WaveWidget->UpdateBossWaveText();
	}
}

void UUIHUD::ShowBossHPBar(bool bShow)
{
	if (bShow)
	{
		if (!BossHPBarWidget && BossHPBarClass)
		{
			BossHPBarWidget = CreateWidget<UBossHPBar>(GetWorld(), BossHPBarClass);
			if (BossHPBarWidget)
			{
				BossHPBarWidget->AddToViewport(100);
			}
		}
		else if (BossHPBarWidget)
		{
			BossHPBarWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
	else
	{
		if (BossHPBarWidget)
		{
			BossHPBarWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UUIHUD::UpdateBossHP(float CurrentHP, float MaxHP)
{
	if (BossHPBarWidget)
	{
		BossHPBarWidget->UpdateBossHP(CurrentHP, MaxHP);
	}
}