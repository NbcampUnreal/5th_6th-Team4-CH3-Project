#include "Entity/AttributeComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UAttributeComponent::UAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();	
}

void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UAttributeComponent::AddAttribute(const FGameplayTag& InTag, float NewBaseValue)
{
	FAttribute NewAttribute;
	NewAttribute.AttributeTag = InTag;
	NewAttribute.BaseValue = NewBaseValue;
	AddAttribute(NewAttribute);
}

void UAttributeComponent::AddAttribute(const FAttribute& NewAttribute)
{
	Attributes.Add(NewAttribute.AttributeTag, NewAttribute);
}

FAttribute* UAttributeComponent::GetAttribute(const FGameplayTag& InTag)
{
	return Attributes.Find(InTag);
}

float UAttributeComponent::GetValue(const FGameplayTag& InTag)
{
	FAttribute* Attribute = GetAttribute(InTag);
	return Attribute == nullptr ? 0.0f : Attribute->GetValue();
}
