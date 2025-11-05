#include "MonsterAttack/GroundAttack.h"
#include "Components/SphereComponent.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Character/MBLPlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/OverlapResult.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MegaBonkLike.h"

AGroundAttack::AGroundAttack()
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);

	RangeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RangeMesh"));
	RangeMesh->SetupAttachment(SceneComponent);
	//RangeMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//RangeMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	//RangeMesh->SetRenderCustomDepth(true);

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(SceneComponent);

	MaxScale = 2.f;
	Duration = 2.f;
	Damage = 20.f;
}

void AGroundAttack::Initialize(AActor* InOwner)
{
	SkillOwner = InOwner;
}

void AGroundAttack::BeginPlay()
{
	Super::BeginPlay();

}


void AGroundAttack::OnTimelineFinished()
{
	TArray<FOverlapResult> Overlaps;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(SkillOwner);

	bool bHit = GetWorld()->OverlapMultiByObjectType(
		Overlaps,
		GetActorLocation(),
		FQuat::Identity,
		ECC_MBL_PLAYER,
		FCollisionShape::MakeSphere(110 * MaxScale)
	);

	if (bHit)
	{
		for (auto& Result : Overlaps)
		{
			if (AMBLPlayerCharacter* Player = Cast<AMBLPlayerCharacter>(Result.GetActor()))
			{
				//데미지적용
				UGameplayStatics::ApplyDamage(Player, Damage, nullptr, SkillOwner, UDamageType::StaticClass());

				//넉백적용
				FVector KnockbackDir = Player->GetActorLocation() - GetActorLocation();
				KnockbackDir.X = 0.f;
				KnockbackDir.Y = 0.f;
				KnockbackDir.Z = 2000.f;
				KnockbackDir.Normalize();

				const float KnockbackStrength = 700.f;
				Player->LaunchCharacter(KnockbackDir * KnockbackStrength, true, true);

				UCharacterMovementComponent* MoveComp = Player->GetCharacterMovement();
				if (MoveComp)
				{
					MoveComp->StopMovementImmediately();
				}
			}
		}
	}

	Destroy();
}