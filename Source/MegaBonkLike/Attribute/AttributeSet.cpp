#include "Attribute/AttributeSet.h"

bool FAttributeSet::HasAttribute(const FGameplayTag& Tag) const
{
	return Attributes.Contains(Tag);
}

float FAttributeSet::GetAttributeValue(const FGameplayTag& Tag) const
{
    if (const FAttribute* Attribute = Attributes.Find(Tag))
    {
        return Attribute->GetValue();
    }
    return 1.0f;
}

void FAttributeSet::AddAttribute(const FGameplayTag& Tag, float InBaseValue)
{
    FAttribute NewAttribute;
    NewAttribute.AttributeTag = Tag;
    NewAttribute.BaseValue = InBaseValue;
    AddAttribute(NewAttribute);
}

void FAttributeSet::AddAttribute(const FAttribute& Attribute)
{
    Attributes.Add(Attribute.AttributeTag, Attribute);
}

void FAttributeSet::AddChangedCallback(const FGameplayTag& Tag, const TFunction<void(const FAttribute&)> NewCallback)
{
    if (Attributes.Contains(Tag))
    {
        Attributes[Tag].AddChangedCallback(NewCallback);
    }
}

int32 FAttributeSet::AddModifier(const FGameplayTag& Tag, const FAttributeModifier& Modifier)
{
    if (Attributes.Contains(Tag))
    {
        return Attributes[Tag].AddModifier(Modifier);
    }
    return -1;
}

void FAttributeSet::ChangeModifier(const FGameplayTag& Tag, int32 InModifierId, const FAttributeModifier& NewModifier)
{
    if (Attributes.Contains(Tag))
    {
        Attributes[Tag].ChangeModifier(InModifierId, NewModifier);
    }
}

void FAttributeSet::RemoveModifier(const FGameplayTag& Tag, int32 ModifierId)
{
    if (Attributes.Contains(Tag))
    {
        Attributes[Tag].RemoveModifier(ModifierId);
    }
}
