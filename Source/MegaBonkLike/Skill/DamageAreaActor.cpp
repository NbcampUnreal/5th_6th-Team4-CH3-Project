#include "Skill/DamageAreaActor.h"
#include "Kismet/GameplayStatics.h"

ADamageAreaActor::ADamageAreaActor()
{
	PrimaryActorTick.bCanEverTick = false;

}
void ADamageAreaActor::BeginPlay()
{
    Super::BeginPlay();
}

void ADamageAreaActor::SetDamage(float InDamage)
{
	Damage = InDamage;
}

void ADamageAreaActor::SetSize(float InSize)
{
	Size = InSize;
	SetActorScale3D(Size * FVector::OneVector);
}

void ADamageAreaActor::SetLifeTime(float InLifeTime)
{
    LifeTime = InLifeTime;
    GetWorldTimerManager().SetTimer(
        LifeTimeHandle,
        this,
        &ThisClass::Shrink,
        0.03f,
        true);
}

void ADamageAreaActor::CheckHit()
{
    TSet<AActor*> AlreadyHit;

    TArray<UPrimitiveComponent*> PrimitiveComponents;
    GetComponents<UPrimitiveComponent>(PrimitiveComponents);
    for (UPrimitiveComponent* PrimComp : PrimitiveComponents)
    {
        if (IsValid(PrimComp) == false || PrimComp->GetGenerateOverlapEvents() == false)
            continue;

        TArray<AActor*> OverlappingActors;
        PrimComp->GetOverlappingActors(OverlappingActors);

        for (AActor* OverlapActor : OverlappingActors)
        {
            // 액터가 유효하지 않으면
            if (IsValid(OverlapActor) == false || OverlapActor == GetInstigator())
                continue;

            if (AlreadyHit.Contains(OverlapActor) == false)
            {
                ApplyDamage(OverlapActor);
                AlreadyHit.Add(OverlapActor);
            }
        }
    }
}

void ADamageAreaActor::ApplyDamage(AActor* TargetActor)
{
    UGameplayStatics::ApplyDamage(TargetActor, Damage, GetInstigatorController(), GetInstigator(), nullptr);
}

void ADamageAreaActor::Shrink()
{
    LifeTimeTimer += 0.03f;
    float Alpha = LifeTimeTimer / LifeTime;
    float Scale = FMath::Lerp(1.f, 0.f, Alpha);
    SetActorScale3D(Scale * Size * FVector::OneVector);
    if (LifeTimeTimer >= LifeTime)
    {
        GetWorldTimerManager().ClearTimer(LifeTimeHandle);
        Destroy();
    }
}