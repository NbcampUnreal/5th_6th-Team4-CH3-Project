#include "Character/MBLCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Game/MBLGameMode.h"

AMBLCharacterBase::AMBLCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMBLCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	SetCameraCollisionIgnore();
	OnDead.AddDynamic(this, &ThisClass::DeadHandle);
}

float AMBLCharacterBase::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float NewDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	UpdateCurrHP(CurrHP - NewDamage);
	OnDamaged.Broadcast(NewDamage, DamageCauser);
	//UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("%s HP: %f"), *GetClass()->GetName(), CurrHP));
	if (CurrHP <= 0.0f)
	{
		OnDead.Broadcast();
	}
	return NewDamage;
}

void AMBLCharacterBase::AddHealth(float Heal)
{
	UpdateCurrHP(CurrHP + Heal);
}

void AMBLCharacterBase::SetMaxHP(float InMaxHP)
{
	float DeltaMaxHp = InMaxHP - MaxHP;
	MaxHP = InMaxHP;
	UpdateCurrHP(CurrHP + DeltaMaxHp);
}

void AMBLCharacterBase::UpdateCurrHP(float InCurrHP)
{
	CurrHP = FMath::Clamp(InCurrHP, 0.0f, MaxHP);
	OnHPChanged.Broadcast(CurrHP, MaxHP);
}

void AMBLCharacterBase::DeadHandle()
{
	if (AMBLGameMode* GameMode = Cast<AMBLGameMode>(GetWorld()->GetAuthGameMode()))
	{
		GameMode->Dead(this);
	}
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
