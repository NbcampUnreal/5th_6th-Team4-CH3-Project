#include "Gimmick/Objects/InteractionObjects/MBLBuffObject.h"
#include "Item/ItemRarityDataRow.h"
#include "Gimmick/Data/ShrineOption.h"
#include "Character/MBLPlayerCharacter.h"
#include "Player/MBLPlayerController.h"
#include "Character/AttributeComponent.h"
#include "Character/InventoryComponent.h"
#include "IngameUI/PopupShrine.h"
#include "Gimmick/Data/BuffObjectDataManager.h"
#include "Components/WidgetComponent.h"

AMBLBuffObject::AMBLBuffObject()
{
	InteractionObjectType = "Shrine";

	ValidComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("ValidComp"));
	ValidComp->SetupAttachment(StaticMeshComp);
	ValidComp->SetWidgetSpace(EWidgetSpace::Screen);
	ValidComp->SetVisibility(true);

}

void AMBLBuffObject::OnObjectActivated(AActor* Activator)
{
	if (bUsed) return;
	if (SantuaryOptions.IsEmpty()) return;

	const TArray<FShrineOption> SelectedOptions = GetRandomOptions();

	if (!SelectedOptions.IsEmpty())
	{
		AMBLPlayerCharacter* Player = Cast<AMBLPlayerCharacter>(Activator);
		if (IsValid(Player) == false)
			return;

		AMBLPlayerController* PlayerController = Cast<AMBLPlayerController>(Player->GetController());
		if (IsValid(PlayerController) == false)
			return;

		UPopupShrine* PopupShrine = Cast<UPopupShrine>(PlayerController->MakePopup(TAG_Popup_Shrine));
		if (IsValid(PopupShrine) == false)
		{
			DestroyObject();
			return;
		}

		PopupShrine->SetInventory(Player->FindComponentByClass<UInventoryComponent>());
		PopupShrine->SetOptions(SelectedOptions);

		PopupShrine->OnSelectedShrine.AddLambda(
			[WeakThis = TWeakObjectPtr<ThisClass>(this)](const FShrineOption& SelectedOption)
			{
				if (WeakThis.IsValid() == false)
					return;

				if (UWorld* World = WeakThis->GetWorld())
				{
					if (UBuffObjectDataManager* BuffObjectDataManager = World->GetSubsystem<UBuffObjectDataManager>())
					{
						BuffObjectDataManager->ApplyShrineBuff(SelectedOption);
					}
				}
			});

		Super::OnObjectActivated(Activator);
		ValidComp->SetVisibility(false);
	}
}

TArray<FGameplayTag> AMBLBuffObject::GetRandomTag() const
{
	TArray<FGameplayTag> OptionTags;
	SantuaryOptions.GetKeys(OptionTags);

	if (OptionTags.Num() < MinTag) return {};

	TArray<FGameplayTag> OptionsToShow;

	for (int i = 0; i < OptionTags.Num(); ++i)
	{
		int32 RandomIdx = FMath::RandRange(0, OptionTags.Num() - 1);
		OptionsToShow.Add(OptionTags[RandomIdx]);
		OptionTags.RemoveAt(RandomIdx);
	}

	return OptionsToShow;
}

TArray<FShrineOption> AMBLBuffObject::GetRandomOptions()
{
	TArray<FShrineOption> SelectedOptions;
	TArray<FGameplayTag> OptionsToShow = GetRandomTag();

	if (OptionsToShow.IsEmpty()) return {};

	for (const FGameplayTag& Tag : OptionsToShow)
	{
		if (FAttributeModifier* Modifier = SantuaryOptions.Find(Tag))
		{
			const FItemRarityDataRow* RandomRarity = GetRandomRarity();
			if (!RandomRarity) return {};
			FShrineOption NewOption;
			NewOption.Rarity = RandomRarity->ItemRarity;
			NewOption.Tag = Tag;
			NewOption.Modifier = *Modifier;
			NewOption.Modifier.Value *= RandomRarity->Multiplier;
			SelectedOptions.Add(NewOption);
		}
	}
	return SelectedOptions;
}

const FItemRarityDataRow* AMBLBuffObject::GetRandomRarity() const
{
	const auto& RarityRows = ItemRarityTable->GetRowMap();
	TArray<FItemRarityDataRow*> RarityArray;
	RarityArray.Reserve(RarityRows.Num());
	float TotalRarityAppearRate = 0.0f;
	for (const auto& Row : RarityRows)
	{
		FItemRarityDataRow* Data = (FItemRarityDataRow*)Row.Value;
		TotalRarityAppearRate += Data->AppearanceRate;
		RarityArray.Add(Data);
	}

	float RandomRate = FMath::RandRange(0.0f, TotalRarityAppearRate);
	for (auto* Rarity : RarityArray)
	{
		RandomRate -= Rarity->AppearanceRate;
		if (RandomRate <= 0.0f)
		{
			return Rarity;
		}
	}
	return RarityArray.IsEmpty() ? nullptr : *RarityArray.begin();
}
