#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PoolBase.generated.h"

UCLASS()
class MEGABONKLIKE_API UPoolBase : public UObject
{
	GENERATED_BODY()
	
public:
	void Initialize(const TSubclassOf<AActor>& InActorClass, int32 InitialSize);
	AActor* GetActor();
	void ReturnActor(AActor* Actor);

	AActor* MakeActor();
	void ActivateActor(AActor* Actor, bool bInActivate);

	TSubclassOf<AActor> GetActorClass() const { return ActorClass; }

protected:
	UPROPERTY()
	TSubclassOf<AActor> ActorClass;
	UPROPERTY()
	TArray<TObjectPtr<AActor>> Actors;
};
