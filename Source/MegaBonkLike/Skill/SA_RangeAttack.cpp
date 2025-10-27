#include "Skill/SA_RangeAttack.h"
#include "Skill/Projectile.h"
#include "Engine/OverlapResult.h"
#include "TimerManager.h"
#include "Kismet/KismetSystemLibrary.h"

void USA_RangeAttack::Activate(TWeakObjectPtr<AActor> InInstigator)
{
	Super::Activate(InInstigator);

	TimerDelegate.BindUObject(this, &ThisClass::StartShoot);
	SetIntervalTimer();
}

void USA_RangeAttack::StartShoot()
{
	ProjectileCount = GetWeaponValue(TAG_Attribute_AttackProjectiles) * GetAttributeValue(TAG_Attribute_AttackProjectiles);
	float Interval = BaseTimerInterval / GetWeaponValue(TAG_Attribute_AttackSpeed) * GetAttributeValue(TAG_Attribute_AttackSpeed);
	AttackInterval = ProjectileCount > 1 ? Interval * 0.5f / (ProjectileCount - 1) : 0.0f;
	ExecuteShoot();
}

void USA_RangeAttack::ExecuteShoot()
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

void USA_RangeAttack::DetectEnemy()
{
	AttackTargets.Empty();

	FVector Origin = Instigator->GetActorLocation();
	TArray<FOverlapResult> Overlaps;
	FCollisionShape CollisionShape;
	CollisionShape.SetSphere(AutoDetectRadius);
	bool bHit = Instigator->GetWorld()->OverlapMultiByChannel(
		Overlaps,
		Origin,
		FQuat::Identity,
		ECC_Pawn,
		CollisionShape);

	if (bHit)
	{
		for (auto& Result : Overlaps)
		{
			AActor* HitActor = Result.GetActor();
			if (IsValid(HitActor) == false || HitActor == Instigator)
				continue;

			if (TargetTag.IsNone() == false && HitActor->ActorHasTag(TargetTag) == false)
				continue;

			// 적의 생존 여부 확인 후 유효한 적 추가

			AttackTargets.Add(HitActor);
		}
	}
}

void USA_RangeAttack::ShootRandomTarget()
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

void USA_RangeAttack::Shoot(const FVector& TargetDir)
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

void USA_RangeAttack::ShootSpread(const FVector& TargetDir)
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

void USA_RangeAttack::ShootSingle(const FVector& TargetDir)
{
	FVector SpawnLocation = Instigator->GetActorLocation() + TargetDir * 30.f;
	FRotator SpawnRotation = TargetDir.Rotation();
	FActorSpawnParameters Params;
	Params.Owner = Instigator.Get();
	Params.Instigator = Cast<APawn>(Instigator.Get());
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AProjectile* Projectile = Instigator->GetWorld()->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, Params);
	if (IsValid(Projectile) == true)
	{
		float ProjectileSpeed = GetWeaponValue(TAG_Attribute_ProjectileSpeed) * GetAttributeValue(TAG_Attribute_ProjectileSpeed);
		Projectile->SetDirectionAndSpeed(TargetDir, ProjectileSpeed);
		Projectile->SetTargetTag(TargetTag);
		float Size = GetWeaponValue(TAG_Attribute_Size) * GetAttributeValue(TAG_Attribute_Size);
		Projectile->SetSize(Size);
		float Damage = GetWeaponValue(TAG_Attribute_Damage) * GetAttributeValue(TAG_Attribute_Damage);
		Projectile->SetDamage(Damage);
	}
}
