#include "MonsterAttack/GroundAttack.h"
#include "Components/SphereComponent.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Character/MBLPlayerCharacter.h"
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
		FCollisionShape::MakeSphere(100.f * MaxScale)
	);

	if (bHit)
	{
		for (auto& Result : Overlaps)
		{
			if (AMBLPlayerCharacter* Player = Cast<AMBLPlayerCharacter>(Result.GetActor()))
			{
				UGameplayStatics::ApplyDamage(Player, Damage, nullptr, SkillOwner, UDamageType::StaticClass());
			}
		}
	}

	Destroy();
}