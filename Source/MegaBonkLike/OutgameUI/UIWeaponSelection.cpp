#include "OutgameUI/UIWeaponSelection.h"
#include "OutgameUI/UIGridItemList.h"
//#include "OutgameUI/UIWeaponInfoPanel.h"
#include "Item/ItemBase.h"
#include "Item/WeaponItem.h"
#include "Item/ItemDataRow.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"

void UUIWeaponSelection::NativeConstruct()
{
    Super::NativeConstruct();

    if (!IsValid(UIGridItemList))
    {
        return;
    }
    //  배열 선언은 한 번만!
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
        //  무기만 필터링
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
    //  UI에 세팅
    UIGridItemList->SetItems(Items);

    UIGridItemList->OnItemClicked.AddDynamic(this, &UUIWeaponSelection::OnWeaponInfoPanel); //델리게이트
}

void UUIWeaponSelection::OnWeaponInfoPanel(UItemBase* ClickedItem)
{
    if (!ClickedItem || !ClickedItem->GetData())
        return;

    const FItemDataRow* Data = ClickedItem->GetData();

    //  선택된 무기 ID 저장
    SelectedWeaponId = Data->ItemId;

    //  오른쪽 정보 패널 업데이트
    //if (WeaponInfoPanel)
    //{
    //    WeaponInfoPanel->SetWeaponInfo(Data);
    //}
}