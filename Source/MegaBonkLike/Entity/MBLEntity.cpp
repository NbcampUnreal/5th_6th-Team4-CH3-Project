#include "Entity/MBLEntity.h"
#include "Entity/AttributeComponent.h"
#include "Kismet/KismetSystemLibrary.h"

AMBLEntity::AMBLEntity()
{
	AttributeComponent = CreateDefaultSubobject<UAttributeComponent>(TEXT("AttributeComponent"));
}

void AMBLEntity::BeginPlay()
{
	Super::BeginPlay();

	SetCameraCollisionIgnore();
}

float AMBLEntity::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float NewDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	CurrHP -= NewDamage;
	UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("%f"), CurrHP));
	return NewDamage;
}

float AMBLEntity::GetAttributeValue(const FGameplayTag& AttributeTag) const
{
	return AttributeComponent == nullptr ? 0.0f : AttributeComponent->GetValue(AttributeTag);
}

void AMBLEntity::SetCameraCollisionIgnore()
{
	TArray<UPrimitiveComponent*> PrimitiveComponents;
	GetComponents<UPrimitiveComponent>(PrimitiveComponents);
	for (UPrimitiveComponent* PrimComp : PrimitiveComponents)
	{
		PrimComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	}
}

