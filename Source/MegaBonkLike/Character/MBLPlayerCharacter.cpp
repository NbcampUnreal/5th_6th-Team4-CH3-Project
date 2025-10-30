#include "Character/MBLPlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "Player/MBLPlayerInputConfig.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/InventoryComponent.h"
#include "Character/AttributeComponent.h"
#include "Character/SkillComponent.h"
#include "Attribute/AttributeTags.h"
#include "Character/CharacterLevelDataRow.h"
#include "Gimmick/Objects/Interface/MBLSpawnObjectInterface.h"
#include "Engine/OverlapResult.h"

AMBLPlayerCharacter::AMBLPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 700.0f;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritRoll = false;
	SpringArm->bDoCollisionTest = true;
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = false;

	Inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
	SkillComponent = CreateDefaultSubobject<USkillComponent>(TEXT("SkillComponent"));

	AttributeComponent = CreateDefaultSubobject<UAttributeComponent>(TEXT("AttributeComponent"));

	NormalSpeed = 600.0f;
	CurrExp = 0.0f;
	MaxExp = 0.0f;
	Level = 1;
	Gold = 0.0f;
	BaseMaxHP = 100.0f;
	InteractRadius = 250.0f;
}

void AMBLPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	AttributeComponent->AddAttribute(EAttributeSourceType::Player, TAG_Attribute_MoveSpeed, 1.0f);
	AttributeComponent->AddAttribute(EAttributeSourceType::Player, TAG_Attribute_Damage, 1.0f);
	AttributeComponent->AddAttribute(EAttributeSourceType::Player, TAG_Attribute_Size, 1.0f);
	AttributeComponent->AddAttribute(EAttributeSourceType::Player, TAG_Attribute_AttackSpeed, 1.0f);
	AttributeComponent->AddAttribute(EAttributeSourceType::Player, TAG_Attribute_ProjectileSpeed, 1.0f);
	AttributeComponent->AddAttribute(EAttributeSourceType::Player, TAG_Attribute_AttackProjectiles, 1.0f);
	AttributeComponent->AddAttribute(EAttributeSourceType::Player, TAG_Attribute_Duration, 1.0f);
	AttributeComponent->AddAttribute(EAttributeSourceType::Player, TAG_Attribute_ExpGain, 1.0f);
	AttributeComponent->AddAttribute(EAttributeSourceType::Player, TAG_Attribute_GoldGain, 1.0f);
	AttributeComponent->AddAttribute(EAttributeSourceType::Player, TAG_Attribute_MaxHP, 1.0f);

	AttributeComponent->AddAttributeChangedCallback(
		TAG_Attribute_MoveSpeed,
		this,
		[WeakThis = TWeakObjectPtr<ThisClass>(this)](const TWeakObjectPtr<UAttribute> Attribute)
		{
			if (WeakThis.IsValid())
				WeakThis->RecalculateSpeed();
		});

	AttributeComponent->AddAttributeChangedCallback(
		TAG_Attribute_MaxHP,
		this,
		[WeakThis = TWeakObjectPtr<ThisClass>(this)](const TWeakObjectPtr<UAttribute> Attribute)
		{
			if (WeakThis.IsValid())
				WeakThis->SetPlayerMaxHP();
		});

	SetLevel(1);
	SetPlayerMaxHP();
	UpdateCurrHP(MaxHP);
}

void AMBLPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (IsValid(EnhancedInputComponent) == true)
	{
		EnhancedInputComponent->BindAction(InputConfig->IA_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
		EnhancedInputComponent->BindAction(InputConfig->IA_Look, ETriggerEvent::Triggered, this, &ThisClass::Input_Look);
		EnhancedInputComponent->BindAction(InputConfig->IA_Jump, ETriggerEvent::Started, this, &ThisClass::Jump);
		EnhancedInputComponent->BindAction(InputConfig->IA_Jump, ETriggerEvent::Completed, this, &ThisClass::StopJumping);
		EnhancedInputComponent->BindAction(InputConfig->IA_Interact, ETriggerEvent::Started, this, &ThisClass::Interact);

		EnhancedInputComponent->BindAction(InputConfig->IA_TempTest, ETriggerEvent::Started, this, &ThisClass::InputTempAcquireItem);
	}
}

void AMBLPlayerCharacter::AddAttributeChangedCallback(const FGameplayTag& Tag, TWeakObjectPtr<UObject> InInstigator, TFunction<void(const TWeakObjectPtr<UAttribute>)> NewCallBack)
{
	if (IsValid(AttributeComponent) == true)
	{
		AttributeComponent->AddAttributeChangedCallback(Tag, InInstigator, NewCallBack);
	}
}

void AMBLPlayerCharacter::RemoveAttributeChangedCallback(const FGameplayTag& Tag, TWeakObjectPtr<UObject> InInstigator)
{
	if (IsValid(AttributeComponent) == true)
	{
		AttributeComponent->RemoveAttributeChangedCallback(Tag, InInstigator);
	}
}

float AMBLPlayerCharacter::GetAttributeValue(const FGameplayTag& AttributeTag) const
{
	return AttributeComponent == nullptr ? 0.0f : AttributeComponent->GetFinalValue(AttributeTag);
}

FVector AMBLPlayerCharacter::GetFootLocation() const
{
	return GetCharacterMovement()->GetActorFeetLocation();
}

void AMBLPlayerCharacter::AcquireExp(float Exp)
{
	CurrExp += Exp * GetAttributeValue(TAG_Attribute_ExpGain);
	while (CurrExp >= MaxExp)
	{
		CurrExp -= MaxExp;
		SetLevel(Level + 1);
	}
	OnExpChanged.Broadcast(CurrExp, MaxExp);
}

void AMBLPlayerCharacter::SetLevel(int32 InLevel)
{
	Level = InLevel;
	OnChangedLevel.Broadcast(Level);

	SetMaxExp();
}

void AMBLPlayerCharacter::AcquireGold(float InGold)
{
	Gold += InGold * GetAttributeValue(TAG_Attribute_GoldGain);
	OnChangedGold.Broadcast(Gold);
}

void AMBLPlayerCharacter::Input_Move(const FInputActionValue& InputValue)
{
	FVector2D MoveVector = InputValue.Get<FVector2D>();

	const FRotator ControlRotation = GetController()->GetControlRotation();
	const FRotator ControlRotationYaw(0.f, ControlRotation.Yaw, 0.f);

	const FVector ForwardVector = FRotationMatrix(ControlRotationYaw).GetUnitAxis(EAxis::X);
	const FVector RightVector = FRotationMatrix(ControlRotationYaw).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardVector, MoveVector.X);
	AddMovementInput(RightVector, MoveVector.Y);
}

void AMBLPlayerCharacter::Input_Look(const FInputActionValue& InputValue)
{
	FVector2D LookVector = InputValue.Get<FVector2D>();

	AddControllerYawInput(LookVector.X);
	AddControllerPitchInput(LookVector.Y);
}

void AMBLPlayerCharacter::Interact(const FInputActionValue& Value)
{
	TArray<FOverlapResult> Overlaps;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(InteractRadius);
	bool bHit = GetWorld()->OverlapMultiByChannel(
		Overlaps,
		GetActorLocation(),
		FQuat::Identity,
		ECC_Pawn,
		Sphere);

	if (bHit)
	{
		for (auto& Result : Overlaps)
		{
			AActor* OverlappedActor = Result.GetActor();
			if (IMBLSpawnObjectInterface* InteractableActor = Cast<IMBLSpawnObjectInterface>(OverlappedActor))
			{
				InteractableActor->OnObjectActivated(this);
				return;
			}
		}
	}

}

void AMBLPlayerCharacter::InputTempAcquireItem()
{
	Inventory->AddOrUpgradeItem(100);
	Inventory->AddOrUpgradeItem(101);
	Inventory->AddOrUpgradeItem(102);
	Inventory->AddOrUpgradeItem(103);
	Inventory->AddOrUpgradeItem(200);
	Inventory->AddOrUpgradeItem(300);
}

void AMBLPlayerCharacter::RecalculateSpeed()
{
	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed * AttributeComponent->GetFinalValue(TAG_Attribute_MoveSpeed);
}

FCharacterLevelDataRow* AMBLPlayerCharacter::GetLevelData(int32 InLevel) const
{
	if (CharacterLevelDataTable == nullptr)
		return nullptr;

	const auto& Rows = CharacterLevelDataTable->GetRowMap();
	for (auto& Row : Rows)
	{
		FCharacterLevelDataRow* Data = (FCharacterLevelDataRow*)Row.Value;
		if (Data->Level == InLevel)
		{
			return Data;
		}
	}
	return nullptr;
}

void AMBLPlayerCharacter::SetMaxExp()
{
	FCharacterLevelDataRow* Data = GetLevelData(Level);
	if (Data == nullptr)
		return;

	MaxExp = Data->MaxExp;
	OnExpChanged.Broadcast(CurrExp, MaxExp);
}

void AMBLPlayerCharacter::SetPlayerMaxHP()
{
	SetMaxHP(BaseMaxHP * GetAttributeValue(TAG_Attribute_MaxHP));
}
