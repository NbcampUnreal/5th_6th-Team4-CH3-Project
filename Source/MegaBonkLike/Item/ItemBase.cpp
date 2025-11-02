#include "Item/ItemBase.h"
#include "Item/ItemDataRow.h"
#include "Character/AttributeComponent.h"

void UItemBase::Init(AActor* InOwner, const FItemDataRow* InData)
{
    Owner = InOwner;
    Data = InData;
}

UAttributeComponent* UItemBase::GetOwnerAttributeComponent()
{
    return Owner.IsValid() == true ? Owner->FindComponentByClass<UAttributeComponent>() : nullptr;
}
