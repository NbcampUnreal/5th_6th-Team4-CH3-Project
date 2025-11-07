#include "Skill/WSA_RangeAttack.h"
#include "Skill/Projectile.h"
#include "Engine/OverlapResult.h"
#include "TimerManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MegaBonkLike.h"
#include "Common/PoolSubsystem.h"
#include "Skill/Projectile/ProjectileActionBase.h"
#include "GameFramework/ProjectileMovementComponent.h"

void UWSA_RangeAttack::Activate(TWeakObjectPtr<AActor> InInstigator)
{
	Super::Activate(InInstigator);

	TimerDelegate.BindUObject(this, &ThisClass::StartShoot);
	SetIntervalTimer();
}

void UWSA_RangeAttack::StartShoot()
{
	ProjectileCount = GetValue(TAG_Attribute_AttackProjectiles);
	float Interval = BaseTimerInterval / GetValue(TAG_Attribute_AttackSpeed);
	AttackInterval = ProjectileCount > 1 ? Interval * 0.5f / (ProjectileCount - 1) : 0.0f;
	ExecuteShoot();
}

void UWSA_RangeAttack::ExecuteShoot()
{
	if (ProjectileCount <= 0)
		return;

	if (Instigator.IsValid() == false || IsValid(ProjectileClass) == false)
		return;

	if (bAutoDetect == false)
	{
		Shoot(Instigator->GetActorForwardVector());
	}
	else
	{
		ShootRandomTarget();
	}
}

void UWSA_RangeAttack::DetectEnemy()
{
	AttackTargets.Empty();

	FVector Origin = Instigator->GetActorLocation();
	TArray<FOverlapResult> Overlaps;
	FCollisionShape CollisionShape;
	CollisionShape.SetSphere(AutoDetectRadius);
	bool bHit = Instigator->GetWorld()->OverlapMultiByObjectType(
		Overlaps,
		Origin,
		FQuat::Identity,
		ECC_MBL_ENEMY,
		CollisionShape);

	if (bHit)
	{
		for (auto& Result : Overlaps)
		{
			AActor* HitActor = Result.GetActor();
			if (IsValid(HitActor) == false || HitActor == Instigator)
				continue;

			// 적의 생존 여부 확인 후 유효한 적 추가

			AttackTargets.Add(HitActor);
		}
	}
}

void UWSA_RangeAttack::ShootRandomTarget()
{
	DetectEnemy();
	if (AttackTargets.IsEmpty())
		return;

	// 가까운 적일 수록 높은 가중치를 주고, 그 값을 바탕으로 랜덤 적 공격
	TArray<float> DistanceWeights;
	float TotalWeight = 0.0f;
	for (const auto& Target : AttackTargets)
	{
		float Weight = 0.5f + (1.0f - FVector::Distance(Instigator->GetActorLocation(), Target->GetActorLocation()) / AutoDetectRadius) * 0.5f;		// 가중치 완화
		DistanceWeights.Add(Weight);
		TotalWeight += Weight;
	}

	float RandomValue = FMath::RandRange(0.0f, TotalWeight);
	int RandomIdx = AttackTargets.Num() - 1;
	for (int i = 0; i < DistanceWeights.Num(); ++i)
	{
		RandomValue -= DistanceWeights[i];
		if (RandomValue <= 0)
		{
			RandomIdx = i;
			break;
		}
	}
	if (AttackTargets[RandomIdx].IsValid() == false)
		return;

	FVector TargetDir = (AttackTargets[RandomIdx]->GetActorLocation() - Instigator->GetActorLocation()).GetSafeNormal();
	Shoot(TargetDir);
}

void UWSA_RangeAttack::Shoot(const FVector& TargetDir)
{
	if (bShootSpread)
	{
		ShootSpread(TargetDir);
		ProjectileCount = 0;
	}
	else
	{
		ShootSingle(TargetDir);
		--ProjectileCount;

		UWorld* World = Instigator->GetWorld();
		if (IsValid(World) == false)
			return;
		World->GetTimerManager().ClearTimer(AttackIntervalHandle);
		World->GetTimerManager().SetTimer(
			AttackIntervalHandle,
			this,
			&ThisClass::ExecuteShoot,
			AttackInterval,
			false);
	}
}

void UWSA_RangeAttack::ShootSpread(const FVector& TargetDir)
{
	if (ProjectileCount == 1)
	{
		ShootSingle(TargetDir);
		return;
	}

	FRotator TargetRot = TargetDir.Rotation();
	float MaxAngle = FMath::Min(6.0f * ProjectileCount, 60.0f);
	float DeltaAngle = MaxAngle * 2.0f / (ProjectileCount - 1);
	for (int i = 0; i < ProjectileCount; ++i)
	{
		float CurrAngle = -MaxAngle + i * DeltaAngle;
		FRotator CurrRot = TargetRot;
		CurrRot.Yaw += CurrAngle;
		ShootSingle(CurrRot.Vector());
	}
}

void UWSA_RangeAttack::ShootSingle(const FVector& TargetDir)
{
	UPoolSubsystem* PoolSubsystem = GetWorld()->GetSubsystem<UPoolSubsystem>();
	if (IsValid(PoolSubsystem) == false)
		return;

	FVector Location = Instigator->GetActorLocation() + TargetDir * 2.f;
	FRotator Rotation = TargetDir.Rotation();
	AProjectile* Projectile = PoolSubsystem->GetFromPool<AProjectile>(ProjectileClass, Location, Rotation);
	if (IsValid(Projectile) == true)
	{
		Projectile->SetOwner(Instigator.Get());
		Projectile->SetInstigator(Cast<APawn>(Instigator.Get()));

		Projectile->SetActorEnableCollision(false);
		Projectile->SetDirectionAndSpeed(TargetDir, GetValue(TAG_Attribute_ProjectileSpeed));
		Projectile->SetSize(GetValue(TAG_Attribute_Size));
		Projectile->SetAttackData(CreateAttackDataBase());
		FProjectileActionContext ActionContext;
		ActionContext.LifeTime = BaseLifeTime * GetValue(TAG_Attribute_Duration);
		ActionContext.ChainCount = BaseChainCount;		// 영향 주는 속성 생기면 적용
		ActionContext.Instigator = Instigator;
		Projectile->SetProjectileAction(ProjectileActionClass, ActionContext);
		Projectile->SetActorEnableCollision(true);
	}
}
