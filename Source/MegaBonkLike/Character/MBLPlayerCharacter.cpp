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
	AttributeComponent->AddAttribute(EAttributeSourceType::Player, TAG_Attribute_MoveSpeed, 1.0f);
	AttributeComponent->AddAttribute(EAttributeSourceType::Player, TAG_Attribute_Damage, 1.0f);
	AttributeComponent->AddAttribute(EAttributeSourceType::Player, TAG_Attribute_Size, 1.0f);
	AttributeComponent->AddAttribute(EAttributeSourceType::Player, TAG_Attribute_AttackSpeed, 1.0f);
	AttributeComponent->AddAttribute(EAttributeSourceType::Player, TAG_Attribute_ProjectileSpeed, 1.0f);
	AttributeComponent->AddAttribute(EAttributeSourceType::Player, TAG_Attribute_AttackProjectiles, 1.0f);

	NormalSpeed = 600.0f;	
}

void AMBLPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	AttributeComponent->AddAttributeChangedCallback(
		EAttributeSourceType::Player,
		TAG_Attribute_MoveSpeed,
		[WeakThis = TWeakObjectPtr<ThisClass>(this)](const FAttribute& Attribute)
		{
			if (WeakThis.IsValid())
				WeakThis->RecalculateSpeed();
		});
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
		EnhancedInputComponent->BindAction(InputConfig->IA_TempTest, ETriggerEvent::Started, this, &ThisClass::InputTempAcquireItem);
	}
}

float AMBLPlayerCharacter::GetAttributeValue(const FGameplayTag& AttributeTag) const
{
	return AttributeComponent == nullptr ? 0.0f : AttributeComponent->GetFinalValue(AttributeTag);
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

void AMBLPlayerCharacter::InputTempAcquireItem()
{
	Inventory->AddOrUpgradeItem(100);
	Inventory->AddOrUpgradeItem(200);
	Inventory->AddOrUpgradeItem(300);
}

void AMBLPlayerCharacter::RecalculateSpeed()
{
	// 다른 요소까지 다 합쳐서 계산한 걸로 바꿔야 함
	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed * AttributeComponent->GetFinalValue(TAG_Attribute_MoveSpeed);
}
