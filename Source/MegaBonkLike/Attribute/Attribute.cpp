#include "Attribute/Attribute.h"

int32 FAttribute::AddModifier(const FAttributeModifier& InModifier)
{
	Modifiers.Add(NextId, InModifier);
	BroadCastChanged();
	return NextId++;
}

int32 FAttribute::AddModifier(EAttributeModifierType InType, float InValue)
{
	FAttributeModifier NewModifier;
	NewModifier.Type = InType;
	NewModifier.Value = InValue;
	return AddModifier(NewModifier);
}

bool FAttribute::ChangeModifier(int32 InModifierId, const FAttributeModifier& NewModifier)
{
	FAttributeModifier* Modifier = Modifiers.Find(InModifierId);
	if (Modifier == nullptr)
		return false;

	*Modifier = NewModifier;
	BroadCastChanged();
	return true;
}

void FAttribute::RemoveModifier(int32 InId)
{
	Modifiers.Remove(InId);
	BroadCastChanged();
}

const FAttributeModifier* FAttribute::GetModifier(int32 InId) const
{
	return Modifiers.Find(InId);
}

float FAttribute::GetValue() const
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

	float Result = (BaseValue + AdditiveVal) * MultiplyVal + FinalAdditiveVal;
	return Result;
}

void FAttribute::AddChangedCallback(TFunction<void(const FAttribute&)> NewCallback)
{
	OnChangedCallbacks.Add(NewCallback);
}

void FAttribute::AddChangedCallback(TFunction<void()> NewCallBack)
{
	OnChangedCallbacks.Add(
		[NewCallBack](const FAttribute&)
		{
			NewCallBack();
		});
}

void FAttribute::BroadCastChanged()
{
	for (const auto& Callback : OnChangedCallbacks)
	{
		Callback(*this);
	}
}