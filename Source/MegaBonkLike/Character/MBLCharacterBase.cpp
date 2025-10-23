#include "Character/MBLCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"

AMBLCharacterBase::AMBLCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMBLCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	SetCameraCollisionIgnore();
}

float AMBLCharacterBase::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float NewDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	CurrHP -= NewDamage;
	// UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("%f"), CurrHP));
	return NewDamage;
}

void AMBLCharacterBase::SetCameraCollisionIgnore()
{
	TArray<UPrimitiveComponent*> PrimitiveComponents;
	GetComponents<UPrimitiveComponent>(PrimitiveComponents);
	for (UPrimitiveComponent* PrimComp : PrimitiveComponents)
	{
		PrimComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	}
}
