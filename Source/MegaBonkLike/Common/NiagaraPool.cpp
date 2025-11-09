#include "Common/NiagaraPool.h"
#include "Common/PoolSpawnable.h"
#include "NiagaraComponent.h"

void UNiagaraPool::Initialize(const TSubclassOf<UObject>& InClass, int32 InitialSize)
{
	Super::Initialize(InClass, InitialSize);

	for (int32 i = 0; i < InitialSize; ++i)
	{
		UNiagaraComponent* NewComponent = MakeComponent();
		if (NewComponent)
		{
			Pool.Add(NewComponent);
		}
	}
}

UObject* UNiagaraPool::Get(const FVector& Location, const FRotator& Rotation)
{
	UNiagaraComponent* PooledComponent = nullptr;

	if (Pool.IsEmpty() == false)
	{
		PooledComponent = Pool.Pop(false);
		if (IsValid(PooledComponent) == false)
			return Get(Location, Rotation);
	}
	else
	{
		PooledComponent = MakeComponent();
	}

	if (IsValid(PooledComponent) == false)
		return nullptr;

	if (PooledComponent->IsRegistered() == false)
	{
		PooledComponent->RegisterComponentWithWorld(GetWorld());		// 이거 없으면 월드에 등록 안돼서 안 보임.
	}

	PooledComponent->SetWorldLocationAndRotation(Location, Rotation);

	if (PooledComponent->OnSystemFinished.IsAlreadyBound(this, &UNiagaraPool::ReturnComponent) == false)
	{
		PooledComponent->OnSystemFinished.AddDynamic(this, &UNiagaraPool::ReturnComponent);
	}

	if (IPoolSpawnable* PoolSpawnable = Cast<IPoolSpawnable>(PooledComponent))
	{
		PoolSpawnable->Activate();
	}

	ActivateComponent(PooledComponent, true);
	return PooledComponent;
}

void UNiagaraPool::Return(UObject* Object)
{
	UNiagaraComponent* NiagaraComponent = Cast<UNiagaraComponent>(Object);
	if (IsValid(NiagaraComponent) == false)
		return;

	NiagaraComponent->OnSystemFinished.RemoveDynamic(this, &UNiagaraPool::ReturnComponent);

	ActivateComponent(NiagaraComponent, false);
	Pool.Add(NiagaraComponent);
}

void UNiagaraPool::ReturnComponent(UNiagaraComponent* Component)
{
	Return(Component);
}

UNiagaraComponent* UNiagaraPool::MakeComponent()
{
	UWorld* World = GetWorld();
	if (IsValid(World) == false)
		return nullptr;

	UNiagaraComponent* NewComponent = NewObject<UNiagaraComponent>(World, ObjectClass);
	if (IsValid(NewComponent) == true)
	{
		NewComponent->RegisterComponentWithWorld(World);		// 이거 없으면 월드에 등록 안돼서 안 보임.
		ActivateComponent(NewComponent, false);

		return NewComponent;
	}

	return nullptr;
}

void UNiagaraPool::ActivateComponent(UNiagaraComponent* NiagaraComponent, bool bInActive)
{
	if (IsValid(NiagaraComponent) == false)
		return;

	if (bInActive)
	{
		NiagaraComponent->SetHiddenInGame(false);
	}
	else
	{
		NiagaraComponent->DeactivateImmediate();
		NiagaraComponent->SetHiddenInGame(true);
	}
}
