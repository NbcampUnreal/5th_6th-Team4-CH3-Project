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

struct FInputActionValue;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnChangedLevel, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnChangedGold, int32);

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
	void SetLevel(int32 InLevel);

	void AcquireGold(float InGold);
	
	FOnChangedProgressValue OnExpChanged;
	FOnChangedLevel OnChangedLevel;
	FOnChangedGold OnChangedGold;

protected:
	void Input_Move(const FInputActionValue& InputValue);
	void Input_Look(const FInputActionValue& InputValue);
	void Interact(const FInputActionValue& Value);
	void InputTempAcquireItem();

	void RecalculateSpeed();

	struct FCharacterLevelDataRow* GetLevelData(int32 InLevel) const;

	void SetMaxExp();
	void SetPlayerMaxHP();

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
	float InteractRadius;
};
