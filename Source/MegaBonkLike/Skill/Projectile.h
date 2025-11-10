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
class UProjectileActionBase;
struct FProjectileActionContext;

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
    void SetProjectileAction(const TSubclassOf<UProjectileActionBase>& ActionClass, const FProjectileActionContext& ActionContext);

    void ApplyDamage(AActor* TargetActor);

    void SetUpdatedComponent();

    UFUNCTION()
    void ReturnToPool();

private:
    void SetLifeTimer(float LifeTime);

protected:
    UFUNCTION()
    void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 BodyIndex, bool bFromSweep, const FHitResult& Hit);
    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<USphereComponent> Collision;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<UStaticMeshComponent> StaticMesh;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<USceneComponent> TrailTransform;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<UProjectileMovementComponent> ProjectileMovement;
    UPROPERTY()
    TObjectPtr<UProjectileActionBase> ProjectileAction;

    UPROPERTY(EditAnywhere)
    TSubclassOf<UNiagaraComponent> TrailTemplate;
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TObjectPtr<UNiagaraSystem> TrailEffect;
    UPROPERTY()
    TObjectPtr<UNiagaraComponent> TrailComponent;
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
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float OriginTrailLifeTime;
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FLinearColor OriginTrailColor;

    FTimerHandle DestroyTimerHandle;

    bool bReturnedToPool;
};
