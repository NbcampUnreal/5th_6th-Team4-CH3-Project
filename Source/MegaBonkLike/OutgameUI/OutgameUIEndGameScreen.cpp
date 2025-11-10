
#include "OutgameUI/OutgameUIEndGameScreen.h"
#include "Components/TextBlock.h"
#include "Character/MBLPlayerCharacter.h"
#include "Character/InventoryComponent.h"
#include "IngameUI/UIHorizontalItemList.h"
#include "Item/ItemEnums.h"
#include "Game/MBLGameState.h"
#include "Game/MBLGameMode.h"
#include "Kismet/GameplayStatics.h"


void UOutgameUIEndGameScreen::NativeConstruct()
{
	Super::NativeConstruct();
}

void UOutgameUIEndGameScreen::SetPlayer(TWeakObjectPtr<AMBLPlayerCharacter> InPlayer)
{
	Player = InPlayer;
	SetOutgameUIScore();
	SetOutgameUIInventory();
}

void UOutgameUIEndGameScreen::SetOutgameUIScore()
{
	if (!Player.IsValid())
	{
		return;
	}

	AMBLGameState* GameState = Cast<AMBLGameState>(UGameplayStatics::GetGameState(GetWorld()));
	AMBLPlayerCharacter* PlayerCharacter = Cast<AMBLPlayerCharacter>(Player.Get());
	int32 Kills = 0;
	float TimeSurvived = 0.f;
	int32 Level = 1;

	if (GameState)
	{
		Kills = GameState->GetKills();
		TimeSurvived = GameState->TimeSurvived();
	}

	if (PlayerCharacter)
	{
		Level = PlayerCharacter->GetLevel();
	}

	if (KillsText)
	{
		KillsText->SetText(FText::FromString(FString::Printf(TEXT("Kills: %d"), Kills)));
	}

	if (TimeText)
	{
		int32 Minutes = FMath::FloorToInt(TimeSurvived / 60.f);
		int32 Seconds = FMath::FloorToInt(FMath::Fmod(TimeSurvived, 60.f));
		TimeText->SetText(FText::FromString(FString::Printf(TEXT("TimeSurvived: %02d:%02d"), Minutes, Seconds)));
	}

	if (LevelText)
	{
		LevelText->SetText(FText::FromString(FString::Printf(TEXT("Level: %d"), Level))); 
	}

	if (TitleText)
	{
		if (GameState->GmaeClear)
		{
			TitleText->SetText(FText::FromString(TEXT("!! Gmae Clea !!")));
		}
		else
		{
			TitleText->SetText(FText::FromString(TEXT("Game Over...")));
		}
	}

}

void UOutgameUIEndGameScreen::SetOutgameUIInventory()
{
	if (Player.IsValid() == false)
	{ 
		return;
	}

	if (UInventoryComponent* Inventory = Player->GetComponentByClass<UInventoryComponent>())
	{
		if (IsValid(WeaponList) == true)
		{
			WeaponList->SetItems(Inventory->GetCachedItems(EItemType::Weapon), Inventory->GetMaxWeaponCount());
		}

		if (IsValid(TomesList) == true)
		{
			TomesList->SetItems(Inventory->GetCachedItems(EItemType::Tomes), Inventory->GetMaxWeaponCount());
		}

		if (IsValid(TomesList) == true)
		{
			MiscList->SetItems(Inventory->GetCachedItems(EItemType::Misc));
		}
	}
}
