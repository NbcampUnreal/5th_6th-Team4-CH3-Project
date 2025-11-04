#include "Skill/SkillActionBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Character/MBLPlayerCharacter.h"

void USkillActionBase::Activate(TWeakObjectPtr<AActor> InInstigator)
{
	Instigator = InInstigator;
}

void USkillActionBase::SetIntervalTimer()
{
    if (Instigator.IsValid() == false)
        return;

    UWorld* World = Instigator->GetWorld();
    if (IsValid(World) == false)
        return;

    // 1회 실행 후 주기적으로 반복
    // 지금 고민중임, Interval attribute value 바뀌었을 떄 타이머를 다시 설정해야 해서 고민중.
    // FTimerHandle도 안쓰게 돼서 바꿀까 고민중.
    TimerDelegate.Execute();
    Instigator->GetWorld()->GetTimerManager().ClearTimer(TimerHandle);    
    float Interval = BaseTimerInterval / GetValue(TAG_Attribute_AttackSpeed);
    World->GetTimerManager().SetTimer(
        TimerHandle,
        this,
        &ThisClass::SetIntervalTimer,
        Interval,
        false);
}

float USkillActionBase::GetValue(const FGameplayTag& AttributeTag) const
{
    auto* Entity = Cast<AMBLPlayerCharacter>(Instigator);
    return Entity == nullptr ? 0.0f : Entity->GetAttributeValue(AttributeTag);
}