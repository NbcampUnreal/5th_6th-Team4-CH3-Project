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
#include "Gimmick/Objects/SpawnObjects/MBLBaseSpawnObject.h"
#include "Components/WidgetComponent.h"
#include "IngameUI/HPBar.h"
#include "MegaBonkLike.h"
#include "Player/MBLPlayerController.h"
#include "Item/ItemSelectOption.h"
#include "IngameUI/PopupItemSelect.h"
#include "IngameUI/PopupTags.h"
#include "IngameUI/PopupItemAcquire.h"
#include "Attack/AttackHandleComponent.h"
#include "Game/MBLGameInstance.h"  //////////////////////////////////////////////////////////////////////////// 시작 아이템 
#include "Game/MBLGameMode.h"

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
	AttackHandleComponent = CreateDefaultSubobject<UAttackHandleComponent>(TEXT("AttackHandleComponent"));

	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBarWidget"));
	HPBarWidget->SetupAttachment(GetMesh());
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	HPBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, -40.0f));
	HPBarWidget->SetDrawSize(FVector2D(200.0f, 40.0f));
	HPBarWidget->SetWidgetClass(HPBarClass);

	NormalSpeed = 600.0f;
	CurrExp = 0.0f;
	MaxExp = 0.0f;
	Level = 1;
	Gold = 0.0f;
	BaseMaxHP = 100.0f;
	InteractRadius = 250.0f;
	BaseAttractRadius = 600.0f;
	BaseJumpVelocity = 560.0f;
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
	AttributeComponent->AddAttribute(EAttributeSourceType::Player, TAG_Attribute_PickupRange, 1.0f);
	AttributeComponent->AddAttribute(EAttributeSourceType::Player, TAG_Attribute_JumpCount, 1.0f);
	AttributeComponent->AddAttribute(EAttributeSourceType::Player, TAG_Attribute_JumpHeight, 1.0f);
	AttributeComponent->AddAttribute(EAttributeSourceType::Player, TAG_Attribute_CriticalChance, 0.0f);
	AttributeComponent->AddAttribute(EAttributeSourceType::Player, TAG_Attribute_CriticalMultiplier, 1.5f);
	AttributeComponent->AddAttribute(EAttributeSourceType::Player, TAG_Attribute_Knockback, 200.0f);
	AttributeComponent->AddAttribute(EAttributeSourceType::Player, TAG_Attribute_LifeSteal, 0.0f);

	SetPlayerAttributeCallbacks();

	SetLevel(1);

	SetPlayerMaxHP();
	UpdateCurrHP(MaxHP);
	if (UHPBar* HPBar = Cast<UHPBar>(HPBarWidget->GetUserWidgetObject()))
	{
		HPBar->UpdateHP(CurrHP, MaxHP);
		OnHPChanged.AddDynamic(HPBar, &UHPBar::UpdateHP);
	}

	SetPlayerMaxJumpCount();
	SetPlayerMaxJumpHeight();

	GetWorldTimerManager().SetTimer(
		AttractItemHandle,
		this,
		&ThisClass::AttractItems,
		0.2f,
		true);

	if (AMBLPlayerController* PlayerController = Cast<AMBLPlayerController>(GetController()))
	{
		PlayerController->UpdatePlayerLevel(Level);
		PlayerController->UpdateXP(CurrExp, MaxExp);
		PlayerController->UpdateCoinCount(Gold);

		OnChangedLevel.AddDynamic(PlayerController, &AMBLPlayerController::UpdatePlayerLevel);
		OnExpChanged.AddDynamic(PlayerController, &AMBLPlayerController::UpdateXP);
		OnChangedGold.AddDynamic(PlayerController, &AMBLPlayerController::UpdateCoinCount);
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////// 시작 아이템
	if (auto GameInstance = Cast<UMBLGameInstance>(GetGameInstance()))
	{
		if (GameInstance->SelectedWeaponId != 0)
		{
			Inventory->AddItem(GameInstance->SelectedWeaponId);
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////
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
		LevelUp();
	}
	OnExpChanged.Broadcast(CurrExp, MaxExp);
}

void AMBLPlayerCharacter::LevelUp()
{
	SetLevel(Level + 1);
	OnChangedLevel.Broadcast(Level);

	AcquireRandomWeaponOrTomes();
}

void AMBLPlayerCharacter::SetLevel(int32 InLevel)
{
	Level = InLevel;
	SetMaxExp();
}

void AMBLPlayerCharacter::AcquireGold(float InGold)
{
	Gold += InGold * GetAttributeValue(TAG_Attribute_GoldGain);
	OnChangedGold.Broadcast(Gold);
}

bool AMBLPlayerCharacter::UseGold(float Price)
{
	if (Price > Gold)
		return false;

	Gold -= Price;
	OnChangedGold.Broadcast(Gold);

	return true;
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
	bool bHit = GetWorld()->OverlapMultiByObjectType(
		Overlaps,
		GetActorLocation(),
		FQuat::Identity,
		ECC_MBL_INTERACTOBJECT,
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
	AcquireRandomWeaponOrTomes();

	// 기타 아이템 랜덤 획득 호출
	/*if (AMBLPlayerController* PlayerController = Cast<AMBLPlayerController>(GetController()))
	{
		if (UPopupItemAcquire* PopupItemAcquire = Cast<UPopupItemAcquire>(PlayerController->MakePopup(TAG_Popup_AcquireItem)))
		{
			PopupItemAcquire->SetInventory(Inventory);
			PopupItemAcquire->SetOption();
		}
	}*/
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

void AMBLPlayerCharacter::SetPlayerAttributeCallbacks()
{
	AddAttributeChangedCallback(
		TAG_Attribute_MoveSpeed,
		this,
		[WeakThis = TWeakObjectPtr<ThisClass>(this)](const TWeakObjectPtr<UAttribute> Attribute)
		{
			if (WeakThis.IsValid())
				WeakThis->RecalculateSpeed();
		});

	AddAttributeChangedCallback(
		TAG_Attribute_MaxHP,
		this,
		[WeakThis = TWeakObjectPtr<ThisClass>(this)](const TWeakObjectPtr<UAttribute> Attribute)
		{
			if (WeakThis.IsValid())
				WeakThis->SetPlayerMaxHP();
		});

	AddAttributeChangedCallback(
		TAG_Attribute_JumpCount,
		this,
		[WeakThis = TWeakObjectPtr<ThisClass>(this)](const TWeakObjectPtr<UAttribute> Attribute)
		{
			if (WeakThis.IsValid())
				WeakThis->SetPlayerMaxJumpCount();
		});

	AddAttributeChangedCallback(
		TAG_Attribute_JumpHeight,
		this,
		[WeakThis = TWeakObjectPtr<ThisClass>(this)](const TWeakObjectPtr<UAttribute> Attribute)
		{
			if (WeakThis.IsValid())
				WeakThis->SetPlayerMaxJumpHeight();
		});
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

void AMBLPlayerCharacter::SetPlayerMaxJumpCount()
{
	JumpMaxCount = GetAttributeValue(TAG_Attribute_JumpCount);
}

void AMBLPlayerCharacter::SetPlayerMaxJumpHeight()
{
	if (UCharacterMovementComponent* Movement = GetCharacterMovement())
	{
		Movement->JumpZVelocity = BaseJumpVelocity * GetAttributeValue(TAG_Attribute_JumpHeight);
	}
}

void AMBLPlayerCharacter::AttractItems()
{
	float Radius = BaseAttractRadius * GetAttributeValue(TAG_Attribute_PickupRange);
	TArray<FOverlapResult> Overlaps;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(Radius);
	bool bHit = GetWorld()->OverlapMultiByObjectType(
		Overlaps,
		GetActorLocation(),
		FQuat::Identity,
		ECC_MBL_INTERACTOBJECT,
		Sphere);

	if (bHit)
	{
		for (auto& Result : Overlaps)
		{
			AActor* OverlappedActor = Result.GetActor();
			if (AMBLBaseSpawnObject* AttractableObject = Cast<AMBLBaseSpawnObject>(OverlappedActor))
			{
				AttractableObject->SetTarget(this);
			}
		}
	}
}

void AMBLPlayerCharacter::AcquireRandomWeaponOrTomes()
{
	if (AMBLPlayerController* PlayerController = Cast<AMBLPlayerController>(GetController()))
	{
		if (UPopupItemSelect* PopupItemSelect = Cast<UPopupItemSelect>(PlayerController->MakePopup(TAG_Popup_SelectItem)))
		{
			PopupItemSelect->SetInventory(Inventory);
			PopupItemSelect->SetOptions(3);
		}
	}
}

void AMBLPlayerCharacter::DeadHandle()
{
	if (AMBLGameMode* GameMode = Cast<AMBLGameMode>(GetWorld()->GetAuthGameMode()))
	{
		GameMode->DeadPlayer();
	}
}
