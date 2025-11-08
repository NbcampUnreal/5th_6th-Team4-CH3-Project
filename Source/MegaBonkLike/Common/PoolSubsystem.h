#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Common/ActorPool.h"
#include "Common/NiagaraPool.h"
#include "PoolSubsystem.generated.h"

UCLASS()
class MEGABONKLIKE_API UPoolSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	template <typename T>
	T* GetFromPool(const TSubclassOf<T>& ObjectClass, const FVector& Location = FVector(), const FRotator& Rotation = FRotator());

	void ReturnToPool(UObject* Object);

private:
	UPROPERTY()
	TMap<TSubclassOf<UObject>, TObjectPtr<UPoolBase>> PoolMap;
};

template<typename T>
inline T* UPoolSubsystem::GetFromPool(const TSubclassOf<T>& ObjectClass, const FVector& Location, const FRotator& Rotation)
{
	if (IsValid(ObjectClass) == false)
		return nullptr;

	TObjectPtr<UPoolBase>* PoolPtr = PoolMap.Find(ObjectClass);
	if (PoolPtr == nullptr)
	{
		UPoolBase* NewPool = nullptr;
		if (TIsDerivedFrom<T, AActor>::Value)
			NewPool = NewObject<UActorPool>(this);
		if (TIsDerivedFrom<T, UNiagaraComponent>::Value)
			NewPool = NewObject<UNiagaraPool>(this);

		if (IsValid(NewPool) == false)
			return nullptr;

		NewPool->Initialize(ObjectClass, 5);
		PoolMap.Add(ObjectClass, NewPool);
		PoolPtr = PoolMap.Find(ObjectClass);
		if (!PoolPtr)
			return nullptr;
	}

	UPoolBase* Pool = PoolPtr->Get();
	if (IsValid(Pool) == false)
		return nullptr;

	return Cast<T>(Pool->Get(Location, Rotation));
}
