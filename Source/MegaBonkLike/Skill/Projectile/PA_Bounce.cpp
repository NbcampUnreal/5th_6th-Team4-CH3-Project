#include "Skill/Projectile/PA_Bounce.h"
#include "Skill/Projectile.h"

void UPA_Bounce::Initialize(TWeakObjectPtr<AProjectile> InOwnerProjectile, const FProjectileActionContext& InContext)
{
	Super::Initialize(InOwnerProjectile, InContext);
    
    if (OwnerProjectile.IsValid() == false)
        return;

    bReturned = false;
    if (UWorld* World = OwnerProjectile->GetWorld())
    {
        World->GetTimerManager().ClearTimer(BounceTimerHandle);
        World->GetTimerManager().SetTimer(
            BounceTimerHandle,
            this,
            &ThisClass::ReturnToInstigator,
            Context.LifeTime * 0.5f,
            false);
    }
}

void UPA_Bounce::OnCollision(AActor* OtherActor, const FHitResult& HitResult, bool bIsOverlap)
{
    if (bIsOverlap == false)
    {
        ReturnToInstigator();
    }
}

void UPA_Bounce::ReturnToInstigator()
{
    if (bReturned)
        return;

    if (OwnerProjectile.IsValid() == false)
        return;

    if (Context.Instigator.IsValid() == false)
    {
        OwnerProjectile->ReturnToPool();
        return;
    }

    if (UWorld* World = OwnerProjectile->GetWorld())
    {
        World->GetTimerManager().ClearTimer(BounceTimerHandle);
    }
        
    FVector Dir = (Context.Instigator->GetActorLocation() - OwnerProjectile->GetActorLocation()).GetSafeNormal();
    float Speed = OwnerProjectile->GetVelocity().Size();
    OwnerProjectile->SetDirectionAndSpeed(Dir, Speed);
    OwnerProjectile->SetActorRotation(Dir.Rotation());
}
