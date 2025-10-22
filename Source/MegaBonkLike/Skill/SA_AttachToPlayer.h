#pragma once

#include "CoreMinimal.h"
#include "Skill/SkillActionBase.h"
#include "SA_AttachToPlayer.generated.h"

UCLASS()
class MEGABONKLIKE_API USA_AttachToPlayer : public USkillActionBase
{
	GENERATED_BODY()

public:
	USA_AttachToPlayer();

	virtual void Activate(TWeakObjectPtr<AActor> InInstigator) override;
	virtual void Tick(float DeltaTime) override;
	virtual void Deactivate() override;

private:
	void UpdateTransform();
	void SetSize();
	void CheckHit();
	void ApplyDamage(AActor* TargetActor);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> AttachedActorClass;
	UPROPERTY()
	TObjectPtr<AActor> AttachedActor;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector LocationOffset;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FRotator RotationOffset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bUseCollisionHit;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BaseDamage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BaseSize;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName TargetTag;
};
