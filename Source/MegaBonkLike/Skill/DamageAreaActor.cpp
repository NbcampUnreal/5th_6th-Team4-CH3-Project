#include "Skill/DamageAreaActor.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "MegaBonkLike.h"
#include "Attack/AttackHandleComponent.h"
#include "Common/PoolSubsystem.h"

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

    CheckHitEffects.SetOwner(this);
    SetOverlapEnable(false);
}

void ADamageAreaActor::Activate()
{
    if (IsValid(CollisionComp) == true)
    {
        CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    }
    CheckHitEffects.SetOwner(this);
}

void ADamageAreaActor::Deactivate()
{
    if (IsValid(CollisionComp) == true)
    {
        CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }

    CheckHitEffects.DeactivateAll();
}

void ADamageAreaActor::SetAttackData(const FAttackData& InAttackData)
{
	AttackData = InAttackData;
}

void ADamageAreaActor::SetSize(float InSize)
{
	Size = InSize;
    CurrSize = Size;
	SetActorScale3D(Size * FVector::OneVector);
}

void ADamageAreaActor::SetLifeTime(float InLifeTime)
{
    LifeTime = InLifeTime;
    LifeTimeTimer = 0.0f;
    GetWorldTimerManager().SetTimer(
        LifeTimeHandle,
        this,
        &ThisClass::Shrink,
        0.03f,
        true);
}

void ADamageAreaActor::SetHitTimer(float Interval)
{
    CheckHitOnNextFrame();

    GetWorldTimerManager().ClearTimer(HitTimerHandle);    
    GetWorldTimerManager().SetTimer(
        HitTimerHandle,
        this,
        &ThisClass::CheckHitOnNextFrame,
        Interval,
        false);
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
    if (AttackData.Causer.IsValid() == false)
        return;

    AActor* Causer = AttackData.Causer.Get();
    AttackData.KnockbackDirection = (TargetActor->GetActorLocation() - Causer->GetActorLocation()).GetSafeNormal();
    UAttackHandleComponent* AttackHandleComponent = Causer->FindComponentByClass<UAttackHandleComponent>();
    if (IsValid(AttackHandleComponent) == true)
    {
        AttackHandleComponent->ExecuteAttack(TargetActor, AttackData);
    }
}

void ADamageAreaActor::Shrink()
{
    LifeTimeTimer += 0.03f;
    float Alpha = LifeTimeTimer / LifeTime;
    CurrSize = FMath::Lerp(1.f, 0.f, Alpha) * Size;
    SetActorScale3D(CurrSize * FVector::OneVector);
    if (LifeTimeTimer >= LifeTime)
    {
        GetWorldTimerManager().ClearTimer(LifeTimeHandle);
        ReturnToPool();
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

    //CheckHitEffects.DeactivateAll();
    CheckHitEffects.ActivateAll(FVector::ZeroVector, FRotator::ZeroRotator, CurrSize);
    SetOverlapEnable(false);
}

void ADamageAreaActor::ReturnToPool()
{
    if (UWorld* World = GetWorld())
    {
        if (UPoolSubsystem* PoolSubSystem = World->GetSubsystem<UPoolSubsystem>())
        {
            PoolSubSystem->ReturnToPool(this);
        }
    }
}
