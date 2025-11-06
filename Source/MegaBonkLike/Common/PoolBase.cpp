#include "Common/PoolBase.h"

void UPoolBase::Initialize(const TSubclassOf<AActor>& InActorClass, int32 InitialSize)
{
	ActorClass = InActorClass;

	for (int32 i = 0; i < InitialSize; ++i)
	{
		AActor* NewActor = MakeActor();

		if (IsValid(NewActor) == true)
		{
			ActivateActor(NewActor, false);
			Actors.Add(NewActor);
		}
	}
}

AActor* UPoolBase::GetActor()
{
	AActor* PooledObject = nullptr;

	if (Actors.IsEmpty() == false)
	{
		TObjectPtr<AActor> ObjectPtr = Actors.Pop(false);
		if (ObjectPtr == nullptr)
			return nullptr;

		PooledObject = ObjectPtr.Get();
	}
	else
	{
		PooledObject = MakeActor();

		if (IsValid(PooledObject) == false)
			return nullptr;
	}

	ActivateActor(PooledObject, true);
	return PooledObject;
}

void UPoolBase::ReturnActor(AActor* Actor)
{
	if (IsValid(Actor) == false)
		return;

	if (Actor->IsA(ActorClass) == false)
	{
		Actor->Destroy();
		return;
	}

	ActivateActor(Actor, false);
	Actors.Add(Actor);
}

AActor* UPoolBase::MakeActor()
{
	UWorld* World = GetWorld();
	if (IsValid(ActorClass.Get()) == false)
		return nullptr;

	if (IsValid(World) == false)
		return nullptr;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FVector SpawnLocation = FVector(0.0f, 0.0f, -10000.0f);
	AActor* Actor = World->SpawnActor<AActor>(ActorClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
	return Actor;
}

void UPoolBase::ActivateActor(AActor* Actor, bool bInActivate)
{
	Actor->SetActorTickEnabled(bInActivate);
	Actor->SetActorEnableCollision(bInActivate);
	Actor->SetActorHiddenInGame(!bInActivate);
}
