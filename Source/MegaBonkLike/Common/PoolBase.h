#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PoolBase.generated.h"

UCLASS(Abstract)
class MEGABONKLIKE_API UPoolBase : public UObject
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(const TSubclassOf<UObject>& InClass, int32 InitialSize);
	virtual UObject* Get(const FVector& Location, const FRotator& Rotation) PURE_VIRTUAL(UPoolBase::Get, return nullptr;);
	virtual void Return(UObject* Object) PURE_VIRTUAL(UPoolBase::Return, );

	TSubclassOf<UObject> GetClass() const { return ObjectClass; }

protected:
	UPROPERTY()
	TSubclassOf<UObject> ObjectClass;
};
