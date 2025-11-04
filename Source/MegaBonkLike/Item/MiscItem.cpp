#include "Item/MiscItem.h"
#include "Item/ItemDataRow.h"
#include "Character/SkillComponent.h"
#include "Skill/Skill.h"

void UMiscItem::Init(AActor* InOwner, const FItemDataRow* InData)
{
	Super::Init(InOwner, InData);
	
	AddSkill();
}

void UMiscItem::Upgrade(const FItemUpgradeContext&)
{
	AddSkill();
}

void UMiscItem::OnDestroy()
{
	if (Owner.IsValid() == false)
		return;

	if (USkillComponent* SkillComponent = Owner->FindComponentByClass<USkillComponent>())
	{
		for (const auto& SkillId : SkillIds)
		{
			SkillComponent->RemoveSkill(SkillId);
		}
	}
}

void UMiscItem::AddSkill()
{
	const FMiscItemDataRow* MiscData = (FMiscItemDataRow*)Data;
	if (MiscData == nullptr)
		return;

	if (Owner.IsValid() == false)
		return;

	if (USkillComponent* SkillComponent = Owner->FindComponentByClass<USkillComponent>())
	{
		USkill* CDO = MiscData->SkillClass->GetDefaultObject<USkill>();
		USkill* NewSkill = DuplicateObject<USkill>(CDO, SkillComponent);
		SkillIds.Add(SkillComponent->AddSkill(NewSkill));
	}
}