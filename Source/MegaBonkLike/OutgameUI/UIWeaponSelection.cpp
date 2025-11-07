#include "OutgameUI/UIWeaponSelection.h"
#include "OutgameUI/UIGridItemList.h"
#include "OutgameUI/UIWeaponInfoPanel.h"
#include "Item/ItemBase.h"
#include "Item/WeaponItem.h"
#include "Item/ItemDataRow.h"
#include "Engine/DataTable.h"
#include "Game/MBLGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UUIWeaponSelection::NativeConstruct()
{
    Super::NativeConstruct();

    SelectedWeaponId = 100;
    if (UMBLGameInstance* GameInstance = Cast<UMBLGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
    {
        GameInstance->SelectedWeaponId = SelectedWeaponId;
    }


    if (!IsValid(UIGridItemList))
    {
        return;
    }
    TArray<TWeakObjectPtr<UItemBase>> Items;

    //  무기 데이터테이블 로드
    UDataTable* WeaponTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Data/WeaponDataTable.WeaponDataTable"));
    if (!WeaponTable)
    {
        return;
    }

    TArray<FItemDataRow*> AllRows;
    WeaponTable->GetAllRows(TEXT("LoadWeaponRows"), AllRows);

    for (auto* Row : AllRows)
    {
        if (Row->ItemType == EItemType::Weapon)
        {
            UWeaponItem* NewItem = NewObject<UWeaponItem>(this);
            if (NewItem)
            {
                NewItem->Init(nullptr, Row);
                Items.Add(NewItem);
            }
        }
    }
    UIGridItemList->SetItems(Items);

    UIGridItemList->OnItemClicked.AddDynamic(this, &UUIWeaponSelection::OnWeaponInfoPanel); //델리게이트
}

void UUIWeaponSelection::OnWeaponInfoPanel(UItemBase* ClickedItem)
{
    if (!ClickedItem)
    {
        return;
    }

    if (!ClickedItem->GetData())
    {
        return;
    }

    SelectedWeaponId = ClickedItem->GetData()->ItemId;

    if (UMBLGameInstance* GameInstance = Cast<UMBLGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
    {
        GameInstance->SelectedWeaponId = SelectedWeaponId;
    }

    if (WeaponInfoPanel)
    {
        WeaponInfoPanel->SetWeaponInfo(ClickedItem);
    }
}