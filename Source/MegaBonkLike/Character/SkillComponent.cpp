#include "Character/SkillComponent.h"
#include "Skill/Skill.h"

USkillComponent::USkillComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USkillComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for (auto& Pair : Skills)
	{
		Pair.Value->Tick(DeltaTime);
	}
}

int32 USkillComponent::AddSkill(USkill* NewSkill)
{
	Skills.Add(NextId, NewSkill);
	NewSkill->Activate(GetOwner());
	return NextId++;
}

void USkillComponent::RemoveSkill(int32 InSkillId)
{
	USkill* Skill = GetSkill(InSkillId);
	if (IsValid(Skill) == true)
	{
		Skill->Deactivate();
	}
	Skills.Remove(InSkillId);
}

USkill* USkillComponent::GetSkill(int32 InSKillId) const
{
	return *Skills.Find(InSKillId);
}

