#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Attribute/Attribute.h"
#include "AttributeComponent.generated.h"

UCLASS( ClassGroup=(Entity), meta=(BlueprintSpawnableComponent) )
class MEGABONKLIKE_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAttributeComponent();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void AddAttribute(const FGameplayTag& InTag, float NewBaseValue);
	void AddAttribute(const FAttribute& NewAttribute);
	FAttribute* GetAttribute(const FGameplayTag& InTag);
	float GetValue(const FGameplayTag& InTag);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FGameplayTag, FAttribute> Attributes;
};
