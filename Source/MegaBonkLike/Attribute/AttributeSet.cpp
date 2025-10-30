#include "Attribute/AttributeSet.h"

bool FAttributeSet::HasAttribute(const FGameplayTag& Tag) const
{
	return Attributes.Contains(Tag);
}

float FAttributeSet::GetAttributeValue(const FGameplayTag& Tag) const
{
    if (const auto& Attribute = Attributes.Find(Tag))
    {
        return (*Attribute)->GetValue();
    }
    return 1.0f;
}

void FAttributeSet::AddAttribute(const FGameplayTag& Tag, float InBaseValue)
{
    if (Attributes.Contains(Tag))
        return;

    UAttribute* NewAttribute = NewObject<UAttribute>();
    NewAttribute->Init(Tag, InBaseValue);
    Attributes.Add(Tag, NewAttribute);
}

void FAttributeSet::AddChangedCallback(const FGameplayTag& Tag, TWeakObjectPtr<UObject> Instigator, TFunction<void(const TWeakObjectPtr<UAttribute>)> NewCallback)
{
    if (Attributes.Contains(Tag))
    {
        Attributes[Tag]->AddChangedCallback(Instigator, NewCallback);
    }
}

void FAttributeSet::RemoveChangedCallback(const FGameplayTag& Tag, TWeakObjectPtr<UObject> Instigator)
{
    if (Attributes.Contains(Tag))
    {
        Attributes[Tag]->RemoveChangedCallback(Instigator);
    }
}

int32 FAttributeSet::AddModifier(const FGameplayTag& Tag, const FAttributeModifier& Modifier)
{
    if (Attributes.Contains(Tag))
    {
        return Attributes[Tag]->AddModifier(Modifier);
    }
    return -1;
}

void FAttributeSet::ChangeModifier(const FGameplayTag& Tag, int32 InModifierId, const FAttributeModifier& NewModifier)
{
    if (Attributes.Contains(Tag))
    {
        Attributes[Tag]->ChangeModifier(InModifierId, NewModifier);
    }
}

void FAttributeSet::RemoveModifier(const FGameplayTag& Tag, int32 ModifierId)
{
    if (Attributes.Contains(Tag))
    {
        Attributes[Tag]->RemoveModifier(ModifierId);
    }
}
