#pragma once

#include "CoreMinimal.h"
#include "Common/PoolBase.h"
#include "ActorPool.generated.h"

UCLASS()
class MEGABONKLIKE_API UActorPool : public UPoolBase
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(const TSubclassOf<UObject>& InClass, int32 InitialSize) override;
	virtual UObject* Get(const FVector& Location, const FRotator& Rotation) override;
	virtual void Return(UObject * Object) override;

	AActor* MakeActor();
	void ActivateActor(AActor* Actor, bool bInActivate);

protected:
	UPROPERTY()
	TArray<TObjectPtr<AActor>> Pool;
};
