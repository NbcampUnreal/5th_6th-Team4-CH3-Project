#pragma once

#include "CoreMinimal.h"
#include "Character/MBLCharacterBase.h"
#include "Gimmick/Data/MBLWaveEnums.h"
#include "EnemyBase.generated.h"


UCLASS()
class MEGABONKLIKE_API AEnemyBase : public AMBLCharacterBase
{
	GENERATED_BODY()
	
public:
	AEnemyBase();

protected:
	virtual void BeginPlay() override;
	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser) override;

	virtual void PlayHitFlash();

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	TObjectPtr<UDataTable> StatDataTable;
public:
	UFUNCTION(BlueprintCallable)
	void SetAttack(EMBLWaveState Wave);
	UFUNCTION(BlueprintCallable)
	virtual void SetSpeed(EMBLWaveState Wave);
	UFUNCTION(BlueprintCallable)
	void SetColor(EMBLWaveState Wave);
	UFUNCTION(BlueprintCallable)
	void SetMaxHealth(EMBLWaveState Wave);
	UFUNCTION(BlueprintCallable)
	void SetAll(EMBLWaveState Wave);


};
