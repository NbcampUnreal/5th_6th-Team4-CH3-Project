#pragma once

#include "CoreMinimal.h"
#include "Character/MBLCharacterBase.h"
#include "GameplayTagContainer.h"
#include "MBLPlayerCharacter.generated.h"

class UMBLPlayerInputConfig;
class USpringArmComponent;
class UCameraComponent;
class UInventoryComponent;
class USkillComponent;
class UAttributeComponent;
class UWidgetComponent;
class UAttackHandleComponent;

struct FInputActionValue;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangedLevel, int32, Level);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangedGold, int32, Gold);

UCLASS()
class MEGABONKLIKE_API AMBLPlayerCharacter : public AMBLCharacterBase
{
	GENERATED_BODY()
	
public:
	AMBLPlayerCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	void AddAttributeChangedCallback(const FGameplayTag& Tag, TWeakObjectPtr<UObject> InInstigator, TFunction<void(const TWeakObjectPtr<class UAttribute>)> NewCallBack);
	void RemoveAttributeChangedCallback(const FGameplayTag& Tag, TWeakObjectPtr<UObject> InInstigator);
	float GetAttributeValue(const FGameplayTag& AttributeTag) const;

	FVector GetFootLocation() const;

	void AcquireExp(float Exp);
	void LevelUp();
	void SetLevel(int32 InLevel);
	int32 GetLevel() const { return Level; }

	void AcquireGold(float InGold);

protected:
	void Input_Move(const FInputActionValue& InputValue);
	void Input_Look(const FInputActionValue& InputValue);
	void Interact(const FInputActionValue& Value);
	void InputTempAcquireItem();

	void RecalculateSpeed();

	struct FCharacterLevelDataRow* GetLevelData(int32 InLevel) const;

	void SetPlayerAttributeCallbacks();
	void SetMaxExp();
	void SetPlayerMaxHP();
	void SetPlayerMaxJumpCount();
	void SetPlayerMaxJumpHeight();

	void AttractItems();

	void AcquireRandomWeaponOrTomes();
	virtual void DeadHandle() override;

public:
	FOnChangedProgressValue OnExpChanged;
	FOnChangedLevel OnChangedLevel;
	FOnChangedGold OnChangedGold;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USpringArmComponent> SpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCameraComponent> Camera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UAttributeComponent> AttributeComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UInventoryComponent> Inventory;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USkillComponent> SkillComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UAttackHandleComponent> AttackHandleComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HPBarWidget;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class UHPBar> HPBarClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMBLPlayerInputConfig> InputConfig;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UDataTable> CharacterLevelDataTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float CurrExp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float MaxExp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 Level;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Gold;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float NormalSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BaseMaxHP;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BaseAttractRadius;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BaseJumpVelocity;

	UPROPERTY(VisibleAnywhere)
	int32 CurrJumpCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float InteractRadius;

	FTimerHandle AttractItemHandle;
};
