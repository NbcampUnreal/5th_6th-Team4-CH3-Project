#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "FlightEnemy.generated.h"

class USphereComponent;
class UFloatingPawnMovement;

UCLASS()
class MEGABONKLIKE_API AFlightEnemy : public APawn
{
	GENERATED_BODY()

public:
	AFlightEnemy();

protected:
	virtual void BeginPlay() override;

private:	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> RootComp;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> DamageCollider;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UFloatingPawnMovement> FloatingPawnMovement;


	TWeakObjectPtr<AActor> Target;

	FVector CurrentDirection;

	FTimerHandle TrackingTimerHandle;
	FTimerHandle MoveTimerHandle;
	FTimerHandle DamageTimerHandle;

	bool bCanDamagePlayer;
	float DamageCooldown;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MoveSpeed = 300.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Attack = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHP = 100.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float CurrHP;

	TObjectPtr<AActor> DamageTarget;

private:
	void UpdateTrack();
	void MoveStep();

	UFUNCTION()
	void OnDamageOverlapBegin(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
	UFUNCTION()
	void OnDamageEndOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	//void ResetCanDamage();
	void DamageTick();
	float TakeDamage(
		float DamageAmount, 
		FDamageEvent const& DamageEvent, 
		AController* EventInstigator, 
		AActor* DamageCauser) override;
	void OnDead();
};
