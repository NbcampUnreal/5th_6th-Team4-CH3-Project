#include "Skill/SkillActionBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Entity/MBLEntity.h"

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
    float Interval = BaseTimerInterval / GetSkillValue(1.0f, TAG_Attribute_AttackSpeed) * GetAttributeValue(TAG_Attribute_AttackSpeed);
    World->GetTimerManager().SetTimer(
        TimerHandle,
        this,
        &ThisClass::SetIntervalTimer,
        Interval,
        false);
}

float USkillActionBase::GetAttributeValue(const FGameplayTag& AttributeTag)
{
    auto* Entity = Cast<AMBLEntity>(Instigator);
    return Entity == nullptr ? 0.0f : Entity->GetAttributeValue(AttributeTag);
}

float USkillActionBase::GetSkillValue(float BaseValue, const FGameplayTag& AttributeTag)
{
    if (!GetModifierFunc)
        return BaseValue;

    const FAttributeModifier* Modifier = GetModifierFunc(AttributeTag);
    if (Modifier == nullptr)
        return BaseValue;

    switch (Modifier->Type)
    {
        case EAttributeModifierType::Multiply: return BaseValue * Modifier->Value;
        default: return BaseValue + Modifier->Value;
    }
}