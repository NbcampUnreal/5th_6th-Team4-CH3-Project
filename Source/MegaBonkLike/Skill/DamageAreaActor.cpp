#include "Skill/DamageAreaActor.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "MegaBonkLike.h"

ADamageAreaActor::ADamageAreaActor()
{
	PrimaryActorTick.bCanEverTick = false;
    
    // 기본값 Sphere
    CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
    CollisionComp->SetupAttachment(GetRootComponent());
    CollisionComp->SetGenerateOverlapEvents(true);
    CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);
    CollisionComp->SetCollisionResponseToChannel(ECC_MBL_ENEMY, ECR_Overlap);
}
void ADamageAreaActor::BeginPlay()
{
    Super::BeginPlay();

    SetOverlapEnable(false);
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

void ADamageAreaActor::CheckHitOnNextFrame()
{
    SetOverlapEnable(true);

    GetWorldTimerManager().SetTimerForNextTick(
        this,
        &ThisClass::CheckHit);
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

void ADamageAreaActor::SetOverlapEnable(bool bInEnable)
{
    if (IsValid(CollisionComp) == false)
        return;

    CollisionComp->SetCollisionEnabled(bInEnable == true ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
}

void ADamageAreaActor::CheckHit()
{
    if (IsValid(CollisionComp) == false || CollisionComp->GetGenerateOverlapEvents() == false)
        return;

    TArray<AActor*> OverlappingActors;
    CollisionComp->GetOverlappingActors(OverlappingActors);
    for (AActor* OverlapActor : OverlappingActors)
    {
        // 액터가 유효하지 않으면
        if (IsValid(OverlapActor) == false || OverlapActor == GetInstigator())
            continue;

        ApplyDamage(OverlapActor);
    }

    SetOverlapEnable(false);
}
