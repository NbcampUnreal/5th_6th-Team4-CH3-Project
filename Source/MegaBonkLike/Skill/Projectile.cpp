#include "Skill/Projectile.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

    Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
    Collision->SetGenerateOverlapEvents(true);
    Collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    Collision->SetCollisionResponseToAllChannels(ECR_Ignore);
    Collision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
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

void AProjectile::SetDamage(float InDamage)
{
    Damage = InDamage;
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

void AProjectile::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 BodyIndex, bool bFromSweep, const FHitResult& Hit)
{
    if (IsValid(OtherActor) == false || OtherActor == GetOwner() || OtherActor == GetInstigator())
        return;

    // 지형에 부딪혔을 때 터지게 할 지 결정해야 함

    UGameplayStatics::ApplyDamage(OtherActor, Damage, GetInstigatorController(), this, nullptr);

    if (IsValid(Trail) == true)
    {
        Trail->Deactivate();
    }

    Destroy();
}


