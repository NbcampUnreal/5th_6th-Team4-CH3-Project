#include "Character/AttributeComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UAttributeComponent::UAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	for (EAttributeSourceType Type : TEnumRange<EAttributeSourceType>())
	{
		AttributeSets.Add(Type, FAttributeSet());
	}
}

void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();	
}

void UAttributeComponent::AddAttribute(EAttributeSourceType SourceType, const FGameplayTag& Tag, float InBaseValue)
{
	if (FAttributeSet* Set = AttributeSets.Find(SourceType))
	{
		if (Set->HasAttribute(Tag) == false)
		{
			Set->AddAttribute(Tag, InBaseValue);
			if (auto* Callbacks = CachedCallbacks.Find(Tag))
			{
				for (const auto& p : *Callbacks)
				{
					Set->AddChangedCallback(Tag, p.Key, p.Value);
				}
			}
		}
	}
}

void UAttributeComponent::AddAttributeChangedCallback(const FGameplayTag& Tag, TWeakObjectPtr<UObject> Instigator, TFunction<void(const TWeakObjectPtr<UAttribute>)> NewCallBack)
{
	CachedCallbacks.FindOrAdd(Tag).Add(Instigator, NewCallBack);

	for (EAttributeSourceType Type : TEnumRange<EAttributeSourceType>())
	{
		if (FAttributeSet* Set = AttributeSets.Find(Type))
		{
			Set->AddChangedCallback(Tag, Instigator, NewCallBack);
		}
	}
}

void UAttributeComponent::RemoveAttributeChangedCallback(const FGameplayTag& Tag, TWeakObjectPtr<UObject> Instigator)
{
	if (auto* Callbacks = CachedCallbacks.Find(Tag))
	{
		Callbacks->Remove(Instigator);
	}

	for (EAttributeSourceType Type : TEnumRange<EAttributeSourceType>())
	{
		if (FAttributeSet* Set = AttributeSets.Find(Type))
		{
			Set->RemoveChangedCallback(Tag, Instigator);
		}
	}
}

int32 UAttributeComponent::AddModifer(EAttributeSourceType SourceType, const FGameplayTag& Tag, const FAttributeModifier& Modifier)
{
	if (FAttributeSet* Set = AttributeSets.Find(SourceType))
	{
		return Set->AddModifier(Tag, Modifier);
	}
	return -1;
}

void UAttributeComponent::ChangeModifier(EAttributeSourceType SourceType, const FGameplayTag& Tag, int32 ModifierId, const FAttributeModifier& Modifier)
{
	if (FAttributeSet* Set = AttributeSets.Find(SourceType))
	{
		Set->ChangeModifier(Tag, ModifierId, Modifier);
	}
}

void UAttributeComponent::RemoveModifier(EAttributeSourceType SourceType, const FGameplayTag& Tag, int32 ModifierId)
{
	if (FAttributeSet* Set = AttributeSets.Find(SourceType))
	{
		Set->RemoveModifier(Tag, ModifierId);
	}
}

float UAttributeComponent::GetAttributeValue(EAttributeSourceType SourceType, const FGameplayTag& Tag) const
{
	if (const FAttributeSet* Set = AttributeSets.Find(SourceType))
	{
		return Set->GetAttributeValue(Tag);
	}
	return 1.0f;
}

float UAttributeComponent::GetFinalValue(const FGameplayTag& Tag) const
{
	float FinalValue = 1.0f;
	for (EAttributeSourceType Type : TEnumRange<EAttributeSourceType>())
	{
		if (AttributeSets.Contains(Type))
		{
			FinalValue *= AttributeSets[Type].GetAttributeValue(Tag);
		}
	}
	return FinalValue;
}
