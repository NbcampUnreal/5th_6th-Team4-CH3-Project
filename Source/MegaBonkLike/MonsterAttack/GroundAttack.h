#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GroundAttack.generated.h"

class USceneComponent;
class USphereComponent;

UCLASS()
class MEGABONKLIKE_API AGroundAttack : public AActor
{
	GENERATED_BODY()
	
public:	
	AGroundAttack();

	void Initialize(AActor* InOwner);

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, Category="Skill")
	float MaxScale;
	UPROPERTY(EditAnywhere, Category = "Skill")
	float Duration;
	UPROPERTY(EditAnywhere, Category = "Skill")
	float Damage;

	UPROPERTY()
	AActor* SkillOwner;

	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> SceneComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> RangeMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent> SphereComponent;

private:

	UFUNCTION(BlueprintCallable)
	void OnTimelineFinished();


};
