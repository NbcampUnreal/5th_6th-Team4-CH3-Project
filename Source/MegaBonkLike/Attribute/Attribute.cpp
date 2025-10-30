#include "Attribute/Attribute.h"

void UAttribute::Init(const FGameplayTag& Tag, float InBaseValue)
{
	AttributeTag = Tag;
	BaseValue = InBaseValue;

	RecalculateFinalValue();
}

int32 UAttribute::AddModifier(const FAttributeModifier& InModifier)
{
	Modifiers.Add(NextModifierId, InModifier);

	RecalculateFinalValue();
	BroadCastChanged();

	return NextModifierId++;
}

int32 UAttribute::AddModifier(EAttributeModifierType InType, float InValue)
{
	FAttributeModifier NewModifier;
	NewModifier.Type = InType;
	NewModifier.Value = InValue;
	return AddModifier(NewModifier);
}

bool UAttribute::ChangeModifier(int32 InModifierId, const FAttributeModifier& NewModifier)
{
	FAttributeModifier* Modifier = Modifiers.Find(InModifierId);
	if (Modifier == nullptr)
		return false;

	*Modifier = NewModifier;

	RecalculateFinalValue();
	BroadCastChanged();
	return true;
}

void UAttribute::RemoveModifier(int32 InId)
{
	Modifiers.Remove(InId);

	RecalculateFinalValue();
	BroadCastChanged();
}

const FAttributeModifier* UAttribute::GetModifier(int32 InId) const
{
	return Modifiers.Find(InId);
}

float UAttribute::GetValue() const
{
	return FinalValue;
}

void UAttribute::RecalculateFinalValue()
{
	float AdditiveVal = 0.0f;
	float MultiplyVal = 1.0f;
	float FinalAdditiveVal = 0.0f;

	for (const auto& [MId, Modifier] : Modifiers)
	{
		switch (Modifier.Type)
		{
			case EAttributeModifierType::Additive:			AdditiveVal += Modifier.Value; break;
			case EAttributeModifierType::Multiply:			MultiplyVal *= (1.0f + Modifier.Value); break;
			case EAttributeModifierType::FinalAdditive:		FinalAdditiveVal += Modifier.Value;	break;
			default: break;
		}
	}

	FinalValue = (BaseValue + AdditiveVal) * MultiplyVal + FinalAdditiveVal;
}

void UAttribute::AddChangedCallback(TWeakObjectPtr<UObject> Instigator, TFunction<void(const TWeakObjectPtr<UAttribute>)> NewCallback)
{
	OnChangedCallbacks.Add(Instigator, NewCallback);
}

void UAttribute::AddChangedCallback(TWeakObjectPtr<UObject> Instigator, TFunction<void()> NewCallBack)
{
	OnChangedCallbacks.Add(
		Instigator,
		[NewCallBack](const TWeakObjectPtr<UAttribute>)
		{
			NewCallBack();
		});
}

void UAttribute::RemoveChangedCallback(TWeakObjectPtr<UObject> Instigator)
{
	OnChangedCallbacks.Remove(Instigator);
}

void UAttribute::BroadCastChanged()
{
	for (const auto& [CallbackId, Callback] : OnChangedCallbacks)
	{
		Callback(this);
	}
}