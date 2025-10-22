#pragma once

#include "CoreMinimal.h"
#include "AttributeModifier.generated.h"

UENUM(BlueprintType)
enum class EAttributeModifierType : uint8
{
	Additive,					// 합연산
	Multiply,					// 곱연산
	FinalAdditive,				// 최종합연산
	
	MAX							UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct MEGABONKLIKE_API FAttributeModifier
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	EAttributeModifierType Type;
	UPROPERTY(EditAnywhere)
	float Value;
};
