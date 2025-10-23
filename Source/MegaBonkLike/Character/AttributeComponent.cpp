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
		Set->AddAttribute(Tag, InBaseValue);
	}
}

void UAttributeComponent::AddAttribute(EAttributeSourceType SourceType, const FAttribute& NewAttribute)
{
	if (FAttributeSet* Set = AttributeSets.Find(SourceType))
	{
		Set->AddAttribute(NewAttribute);
	}
}

void UAttributeComponent::AddAttributeChangedCallback(EAttributeSourceType SourceType, const FGameplayTag& Tag, TFunction<void(const FAttribute&)> NewCallBack)
{
	if (FAttributeSet* Set = AttributeSets.Find(SourceType))
	{
		Set->AddChangedCallback(Tag, NewCallBack);
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
