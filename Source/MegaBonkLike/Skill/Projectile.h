#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UNiagaraComponent;
class UNiagaraSystem;
class UProjectileMovementComponent;

UCLASS()
class MEGABONKLIKE_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectile();

protected:
	virtual void BeginPlay() override;

public:
    void SetDirectionAndSpeed(const FVector& InDirection, float InSpeed);
    void SetTargetTag(const FName& NewTargetTag);
    void SetDamage(float InDamage);

protected:
    UFUNCTION()
    void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 BodyIndex, bool bFromSweep, const FHitResult& Hit);

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<USphereComponent> Collision;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<UStaticMeshComponent> StaticMesh;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<UNiagaraComponent> Trail;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UNiagaraSystem> TrailEffect;
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    float Damage;
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    float Speed;
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    FName TargetTag;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float LifeTime = 5.0f;

    FTimerHandle DestroyTimerHandle;
};
