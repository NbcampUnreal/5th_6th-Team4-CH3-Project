#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Attribute/AttributeSet.h"
#include "AttributeComponent.generated.h"

UENUM(BlueprintType)
enum class EAttributeSourceType : uint8
{
	Player,				// 유물, 기타 아이템은 Player Attribute에 직접 영향
	Sanctuary,			// 성소
	Buff,				// 맵 버프 석상
	
	MAX
};

ENUM_RANGE_BY_COUNT(EAttributeSourceType, EAttributeSourceType::MAX)

UCLASS( ClassGroup=(Player), meta=(BlueprintSpawnableComponent) )
class MEGABONKLIKE_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAttributeComponent();

	virtual void BeginPlay() override;

	void AddAttribute(EAttributeSourceType SourceType, const FGameplayTag& Tag, float InBaseValue);

	void AddAttributeChangedCallback(const FGameplayTag& Tag, TWeakObjectPtr<UObject> Instigator, TFunction<void(const TWeakObjectPtr<UAttribute>)> NewCallBack);
	void RemoveAttributeChangedCallback(const FGameplayTag& Tag, TWeakObjectPtr<UObject> Instigator);

	int32 AddModifer(EAttributeSourceType SourceType, const FGameplayTag& Tag, const FAttributeModifier& Modifier);
	void ChangeModifier(EAttributeSourceType SourceType, const FGameplayTag& Tag, int32 ModifierId, const FAttributeModifier& Modifier);
	void RemoveModifier(EAttributeSourceType SourceType, const FGameplayTag& Tag, int32 ModifierId);

	float GetAttributeValue(EAttributeSourceType SourceType, const FGameplayTag& Tag) const;
	float GetFinalValue(const FGameplayTag& Tag) const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<EAttributeSourceType, FAttributeSet> AttributeSets;

	// 콜백 함수 먼저 추가되고, Attribute가 나중에 추가될 때를 대비해서 콜백 저장
	// AttributeSet에 새 Attribute가 추가 될 때, 기존에 추가된 콜백이 있으면 등록
	TMap<FGameplayTag, TMap<TWeakObjectPtr<UObject>, TFunction<void(const TWeakObjectPtr<UAttribute>)>>> CachedCallbacks;
};
