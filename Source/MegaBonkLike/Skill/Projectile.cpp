#include "Skill/Projectile.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MegaBonkLike.h"
#include "Attack/AttackHandleComponent.h"

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
    ProjectileMovement->bShouldBounce = false;
    ProjectileMovement->ProjectileGravityScale = 0.0f;

    Trail = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Trail"));
    Trail->SetupAttachment(RootComponent);
}

void AProjectile::BeginPlay()
{
    Super::BeginPlay();

    if (IsValid(TrailEffect) == true)
    {
        Trail->SetAsset(TrailEffect);
        Trail->Activate();
    }
}

void AProjectile::SetDirectionAndSpeed(const FVector& InDirection, float InSpeed)
{
    Speed = InSpeed;
    ProjectileMovement->InitialSpeed = InSpeed;
    ProjectileMovement->MaxSpeed = InSpeed;
    ProjectileMovement->Velocity = InDirection * InSpeed;
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
    if (IsValid(Trail) == true)
    {
        Trail->SetVariableFloat(TEXT("User.Width"), Size * OriginTrailWidth);
    }
}

void AProjectile::SetPenetrate(bool bInPenetrate)
{
    bPenetrate = bInPenetrate;
}

void AProjectile::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 BodyIndex, bool bFromSweep, const FHitResult& Hit)
{
    if (IsValid(OtherActor) == false || OtherActor == GetOwner() || OtherActor == GetInstigator())
        return;

    ApplyDamage(OtherActor);

    if (bPenetrate == false)
    {
        if (IsValid(Trail) == true)
        {
            Trail->Deactivate();
        }
        Destroy();
    }
}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    Destroy();
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
