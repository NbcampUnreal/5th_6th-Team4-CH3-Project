#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Attack/AttackData.h"
#include "Common/PoolSpawnable.h"
#include "Projectile.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UNiagaraComponent;
class UNiagaraSystem;
class UProjectileMovementComponent;

UCLASS(Abstract)
class MEGABONKLIKE_API AProjectile : public AActor, public IPoolSpawnable
{
	GENERATED_BODY()
	
public:	
	AProjectile();

    virtual void Activate() override;
    virtual void Deactivate() override;

protected:
	virtual void BeginPlay() override;

public:
    void SetDirectionAndSpeed(const FVector& InDirection, float InSpeed);
    void SetAttackData(const FAttackData& InAttackData);
    void SetSize(float InSize);
    void SetPenetrate(bool bInPenetrate);

    void SetLifeTimer();

    void ApplyDamage(AActor* TargetActor);

    void ReturnToPool();

protected:
    UFUNCTION()
    void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 BodyIndex, bool bFromSweep, const FHitResult& Hit);
    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<USphereComponent> Collision;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<UStaticMeshComponent> StaticMesh;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<UNiagaraComponent> Trail;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TObjectPtr<UNiagaraSystem> TrailEffect;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    FAttackData AttackData;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    float Speed;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    float Size;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool bPenetrate;
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float OriginTrailWidth;

    FTimerHandle DestroyTimerHandle;

    static const float LifeTime;
};
