#include "Skill/Projectile.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MegaBonkLike.h"
#include "Attack/AttackHandleComponent.h"
#include "Common/PoolSubsystem.h"
#include "Skill/Projectile/ProjectileActionIncludes.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

    Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
    Collision->SetGenerateOverlapEvents(true);
    Collision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    Collision->SetCollisionResponseToAllChannels(ECR_Ignore);
    Collision->SetCollisionResponseToChannel(ECC_MBL_ENEMY, ECR_Overlap);
    Collision->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
    Collision->OnComponentHit.AddDynamic(this, &ThisClass::OnHit);
    Collision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlap);
    RootComponent = Collision;

    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    StaticMesh->SetupAttachment(RootComponent);

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->SetUpdatedComponent(Collision);
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->bShouldBounce = true;
    ProjectileMovement->Bounciness = 1.0f;
    ProjectileMovement->Friction = 0.0f;
    ProjectileMovement->ProjectileGravityScale = 0.0f;

    TrailTransform = CreateDefaultSubobject<USceneComponent>(TEXT("TrailTransform"));

    TrailTemplate = UNiagaraComponent::StaticClass();
}

void AProjectile::BeginPlay()
{
    Super::BeginPlay();

    MovingEffect.SetOwner(this);
}

void AProjectile::SetDirectionAndSpeed(const FVector& InDirection, float InSpeed)
{
    SetUpdatedComponent();

    ProjectileMovement->Deactivate();
    ProjectileMovement->StopMovementImmediately();
    Speed = InSpeed;
    ProjectileMovement->InitialSpeed = InSpeed;
    ProjectileMovement->MaxSpeed = InSpeed;
    ProjectileMovement->Velocity = InDirection * InSpeed;
    ProjectileMovement->Activate(true);
}

void AProjectile::SetAttackData(const FAttackData& InAttackData)
{
    AttackData = InAttackData;
}

void AProjectile::SetSize(float InSize)
{
    Size = InSize;
    FVector NewScale = InSize * FVector::OneVector;
    SetActorScale3D(NewScale);
    if (IsValid(TrailComponent) == true)
    {
        TrailComponent->SetVariableFloat(TEXT("User.Width"), Size * OriginTrailWidth);
    }
}

void AProjectile::SetProjectileAction(const TSubclassOf<UProjectileActionBase>& ActionClass, const FProjectileActionContext& ActionContext)
{
    if (ProjectileAction == nullptr || ProjectileAction->GetClass() != ActionClass)
    {
        ProjectileAction = NewObject<UProjectileActionBase>(this, ActionClass);
    }

    SetLifeTimer(ActionContext.LifeTime);
    if (IsValid(ProjectileAction) == true)
    {
        ProjectileAction->Initialize(this, ActionContext);
    }
}

void AProjectile::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 BodyIndex, bool bFromSweep, const FHitResult& Hit)
{
    if (IsValid(OtherActor) == false || OtherActor == GetOwner() || OtherActor == GetInstigator())
        return;

    ApplyDamage(OtherActor);

    if (IsValid(ProjectileAction) == true)
    {
        ProjectileAction->OnCollision(OtherActor, Hit, true);
    }
}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    SetActorLocation(Hit.Location + Hit.ImpactNormal * 1.0f, false, nullptr, ETeleportType::TeleportPhysics);

    if (IsValid(ProjectileAction) == true)
    {
        ProjectileAction->OnCollision(OtherActor, Hit, false);
    }
}

void AProjectile::Activate()
{
    if (IsValid(ProjectileMovement) == true)
    {
        SetUpdatedComponent();
        ProjectileMovement->StopMovementImmediately();
        ProjectileMovement->Activate(true);
    }

    if (IsValid(Collision) == true)
    {
        Collision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    }

    if (IsValid(StaticMesh) == true)
    {
        StaticMesh->SetHiddenInGame(false);
    }

    GetRootComponent()->UpdateComponentToWorld();

    if (IsValid(TrailComponent) == false)
    {
        UPoolSubsystem* PoolSubsystem = GetWorld()->GetSubsystem<UPoolSubsystem>();
        if (IsValid(PoolSubsystem) == true)
        {
            auto* NewTrail = PoolSubsystem->GetFromPool<UNiagaraComponent>(TrailTemplate, GetActorLocation(), GetActorRotation());
            TrailComponent = IsValid(NewTrail) == true ? NewTrail : nullptr;
        }
    }

    if (IsValid(TrailComponent) == true)
    {
        TrailComponent->SetAsset(TrailEffect);
        TrailComponent->ResetSystem();
        TrailComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
        TrailComponent->SetRelativeLocation(TrailTransform->GetRelativeLocation());
        TrailComponent->SetRelativeRotation(TrailTransform->GetRelativeRotation());
        TrailComponent->UpdateComponentToWorld();
        TrailComponent->Activate(true);
        TrailComponent->SetVariableFloat(TEXT("User.LifeTime"), OriginTrailLifeTime);
        TrailComponent->SetVariableLinearColor(TEXT("User.LinearColor"), OriginTrailColor);
    }

    MovingEffect.SetOwner(this);
    MovingEffect.ActivateAll();

    bReturnedToPool = false;
}

void AProjectile::Deactivate()
{
    if (IsValid(ProjectileMovement) == true)
    {
        ProjectileMovement->StopMovementImmediately();
        ProjectileMovement->Velocity = FVector::ZeroVector;
        ProjectileMovement->Deactivate();
    }

    if (IsValid(Collision) == true)
    {
        Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }

    if (IsValid(StaticMesh) == true)
    {
        StaticMesh->SetHiddenInGame(true);
    }

    if (IsValid(TrailComponent) == true)
    {
        TrailComponent->Deactivate();
    }

    MovingEffect.DeactivateAll();

    bReturnedToPool = true;
    GetWorldTimerManager().ClearTimer(DestroyTimerHandle);
}

void AProjectile::ApplyDamage(AActor* TargetActor)
{
    if (AttackData.Causer.IsValid() == false)
        return;

    AActor* Causer = AttackData.Causer.Get();
    if (IsValid(ProjectileMovement) == true && ProjectileMovement->Velocity.IsNearlyZero() == false)
    {
        AttackData.KnockbackDirection = ProjectileMovement->Velocity.GetSafeNormal();
    }
    else
    {
        AttackData.KnockbackDirection = (TargetActor->GetActorLocation() - Causer->GetActorLocation()).GetSafeNormal();
    }

    UAttackHandleComponent* AttackHandleComponent = Causer->FindComponentByClass<UAttackHandleComponent>();
    if (IsValid(AttackHandleComponent) == true)
    {
        AttackHandleComponent->ExecuteAttack(TargetActor, AttackData);
    }
}

void AProjectile::SetUpdatedComponent()
{
    if (ProjectileMovement->UpdatedComponent == nullptr)
    {
        ProjectileMovement->SetUpdatedComponent(GetRootComponent());
    }
}

void AProjectile::ReturnToPool()
{
    if (bReturnedToPool)
        return;

    if (IsValid(TrailComponent) == true)
    {
        TrailComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
        TrailComponent = nullptr;
    }

    UPoolSubsystem* PoolSubsystem = GetWorld()->GetSubsystem<UPoolSubsystem>();
    if (IsValid(PoolSubsystem) == true)
    {
        PoolSubsystem->ReturnToPool(this);
    }
}

void AProjectile::SetLifeTimer(float LifeTime)
{
    GetWorldTimerManager().SetTimer(
        DestroyTimerHandle,
        this,
        &ThisClass::ReturnToPool,
        LifeTime,
        false);
}