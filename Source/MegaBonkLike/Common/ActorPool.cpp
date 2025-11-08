#include "Common/ActorPool.h"
#include "Common/PoolSpawnable.h"

void UActorPool::Initialize(const TSubclassOf<UObject>& InClass, int32 InitialSize)
{
	Super::Initialize(InClass, InitialSize);

	for (int32 i = 0; i < InitialSize; ++i)
	{
		AActor* NewActor = MakeActor();
		if (IsValid(NewActor) == true)
		{
			ActivateActor(NewActor, false);
			Pool.Add(NewActor);
		}
	}
}

UObject* UActorPool::Get(const FVector& Location, const FRotator& Rotation)
{
	AActor* PooledActor = nullptr;

	if (Pool.IsEmpty() == false)
	{
		PooledActor = Pool.Pop(false);
		if (IsValid(PooledActor) == false)
			return Get(Location, Rotation);
	}
	else
	{
		PooledActor = MakeActor();
	}

	if (IsValid(PooledActor) == false)
		return nullptr;

	PooledActor->SetActorLocation(Location);
	PooledActor->SetActorRotation(Rotation);
	ActivateActor(PooledActor, true);

	if (IPoolSpawnable* PoolSpawnable = Cast<IPoolSpawnable>(PooledActor))
	{
		PoolSpawnable->Activate();
	}

	return PooledActor;
}

void UActorPool::Return(UObject* Object)
{
	AActor* Actor = Cast<AActor>(Object);
	if (IsValid(Actor) == false)
		return;

	if (Actor->IsA(ObjectClass) == false)
	{
		Actor->Destroy();
		return;
	}

	ActivateActor(Actor, false);
	Pool.Add(Actor);
}

AActor* UActorPool::MakeActor()
{
	UWorld* World = GetWorld();
	if (IsValid(World) == false)
		return nullptr;

	if (IsValid(ObjectClass) == false)
		return nullptr;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FVector SpawnLocation = FVector(0.0f, 0.0f, -10000.0f);
	AActor* Actor = World->SpawnActor<AActor>(ObjectClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
	return Actor;
}

void UActorPool::ActivateActor(AActor* Actor, bool bInActivate)
{
	Actor->SetActorTickEnabled(bInActivate);
	Actor->SetActorEnableCollision(bInActivate);
	Actor->SetActorHiddenInGame(!bInActivate);
}
