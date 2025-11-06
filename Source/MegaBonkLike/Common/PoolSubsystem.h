#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Common/PoolBase.h"
#include "Common/PoolSpawnable.h"
#include "PoolSubsystem.generated.h"

UCLASS()
class MEGABONKLIKE_API UPoolSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	template <typename T>
	T* GetFromPool(const TSubclassOf<T>& ActorClass, const FVector& Location, const FRotator& Rotation);

	void ReturnToPool(AActor* Actor);

private:
	UPROPERTY()
	TMap<TSubclassOf<AActor>, TObjectPtr<UPoolBase>> PoolMap;
};

template<typename T>
inline T* UPoolSubsystem::GetFromPool(const TSubclassOf<T>& ActorClass, const FVector& Location, const FRotator& Rotation)
{
	TObjectPtr<UPoolBase>* PoolPtr = PoolMap.Find(ActorClass);
	if (PoolPtr == nullptr)
	{
		UPoolBase* NewPool = NewObject<UPoolBase>(this);
		NewPool->Initialize(ActorClass, 5);
		PoolMap.Add(ActorClass, NewPool);

		PoolPtr = PoolMap.Find(ActorClass);
		if (!PoolPtr)
			return nullptr;
	}

	UPoolBase* Pool = PoolPtr->Get();
	if (IsValid(Pool) == false)
		return nullptr;

	AActor* PooledActorBase = Pool->GetActor();
	if (PooledActorBase == nullptr)
		return nullptr;

	PooledActorBase->SetActorLocation(Location);
	PooledActorBase->SetActorRotation(Rotation);
	if (IPoolSpawnable* PoolSpawnable = Cast<IPoolSpawnable>(PooledActorBase))
	{
		PoolSpawnable->Activate();
	}

	T* PooledActor = Cast<T>(PooledActorBase);
	checkf(IsValid(PooledActor), TEXT("Wrong Pooling"));

	return PooledActor;
}
