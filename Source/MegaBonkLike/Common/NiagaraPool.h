#pragma once

#include "CoreMinimal.h"
#include "Common/PoolBase.h"
#include "NiagaraPool.generated.h"

class UNiagaraComponent;

UCLASS()
class MEGABONKLIKE_API UNiagaraPool : public UPoolBase
{
	GENERATED_BODY()

public:
	virtual void Initialize(const TSubclassOf<UObject>& InClass, int32 InitialSize) override;
	virtual UObject* Get(const FVector& Location, const FRotator& Rotation) override;
	virtual void Return(UObject* Object) override;
	
	UFUNCTION()
	void ReturnComponent(UNiagaraComponent* Component);

	UNiagaraComponent* MakeComponent();
	void ActivateComponent(UNiagaraComponent* NiagaraComponent, bool bInActive);

protected:
	UPROPERTY()
	TArray<TObjectPtr<UNiagaraComponent>> Pool;
};
