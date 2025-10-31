#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UNiagaraComponent;
class UNiagaraSystem;
class UProjectileMovementComponent;

UCLASS(Abstract)
class MEGABONKLIKE_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectile();

protected:
	virtual void BeginPlay() override;

public:
    void SetDirectionAndSpeed(const FVector& InDirection, float InSpeed);
    void SetDamage(float InDamage);
    void SetSize(float InSize);
    void SetPenetrate(bool bInPenetrate);

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
    float Damage;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    float Speed;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    float Size;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool bPenetrate;
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float OriginTrailWidth;

    FTimerHandle DestroyTimerHandle;
};
