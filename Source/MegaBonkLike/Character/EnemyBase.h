#pragma once

#include "CoreMinimal.h"
#include "Character/MBLCharacterBase.h"
#include "EnemyBase.generated.h"



UCLASS()
class MEGABONKLIKE_API AEnemyBase : public AMBLCharacterBase
{
	GENERATED_BODY()
	
public:
	AEnemyBase();

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float WalkSpeed = 300.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float RunSpeed = 600.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float Attack;
	UPROPERTY()
	UMaterialInstanceDynamic* DynamicMaterial;
	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> DynamicMaterials;

public:
	UFUNCTION(BlueprintCallable)
	void SetAttack(float NewAttack);
	UFUNCTION(BlueprintCallable)
	virtual void SetSpeed(float NewSpeed);
	UFUNCTION(BlueprintCallable)
	void SetColor(const FLinearColor& NewColor);



};
