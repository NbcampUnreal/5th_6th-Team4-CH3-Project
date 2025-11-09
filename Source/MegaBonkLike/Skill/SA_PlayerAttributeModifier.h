#pragma once

#include "CoreMinimal.h"
#include "Skill/SkillActionBase.h"
#include "GameplayTagContainer.h"
#include "Attribute/AttributeModifier.h"
#include "SA_PlayerAttributeModifier.generated.h"

UENUM(BlueprintType)
enum class EAttributeTriggerType : uint8
{
    None,                               // 조건 없음
    Player_OnDamaged,                   // 피격 시
    Player_OnKillCountOver,             // 적 일정 수 이상 처치 시
    Player_OnEvery_N_Kills,             // 적 킬 N번마다 적용
    Player_LowerHP,                     // 체력이 임계값 이하 시
    Player_UpperHP,                     // 체력이 임계값 이상 시
};

USTRUCT()
struct FAttributeTrigger
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere)
    EAttributeTriggerType TriggerType;
    UPROPERTY(EditAnywhere)
    float TriggerValue;                     // 사용하는 경우만
    UPROPERTY(EditAnywhere)
    FGameplayTag AttributeTag;
    UPROPERTY(EditAnywhere)
    FAttributeModifier AttributeModifier;
    UPROPERTY(EditAnywhere)
    float Duration;                         // 0.0f면 상시 적용
};

USTRUCT()
struct FTriggeredModifierInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere)
    FGameplayTag AttributeTag;
    UPROPERTY(EditAnywhere)
    int32 ModifierId;
    UPROPERTY()
    FTimerHandle DurationTimerHandle;
};

UCLASS()
class MEGABONKLIKE_API USA_PlayerAttributeModifier : public USkillActionBase
{
	GENERATED_BODY()
	
public:
    virtual void Activate(TWeakObjectPtr<AActor> InInstigator);
    virtual void Deactivate() override;

private:
    UFUNCTION()
    void HandleAllModifiers();

    UFUNCTION()
    void HandlePlayerDamaged(float Damage, AActor* DamageCauser);
    UFUNCTION()
    void HandleKilled(int32 KillCount);
    UFUNCTION()
    void HandleHPChanged(float CurrHP, float MaxHP);

    FAttributeModifier GetBaseCalculatedModifierInfo(int32 TriggerIndex) const;

    void ApplyModifier(int32 TriggerIndex, const FAttributeModifier& CalculatedModifier);
    void RemoveModifier(int32 TriggerIndex);

public:
    UPROPERTY(EditAnywhere)
    TArray<FAttributeTrigger> AttributeTriggers;
    UPROPERTY()
    TMap<int32, FTriggeredModifierInfo> TriggeredModifierInfos;                // (AttributeTrigger Index, PlayerAttributeModifierId)
};
