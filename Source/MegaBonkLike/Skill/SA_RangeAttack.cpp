#include "Skill/SA_RangeAttack.h"
#include "Skill/Projectile.h"
#include "Engine/OverlapResult.h"
#include "TimerManager.h"
#include "Kismet/KismetSystemLibrary.h"

void USA_RangeAttack::Activate(TWeakObjectPtr<AActor> InInstigator)
{
	Super::Activate(InInstigator);
	TimerDelegate.BindUObject(this, &ThisClass::DetectEnemy);

	SetIntervalTimer();
}

void USA_RangeAttack::DetectEnemy()
{
	if (bAutoDetect == false || Instigator.IsValid() == false)
		return;

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

			AttackTargets.Add(HitActor);
		}
	}

	float Interval = BaseTimerInterval / GetWeaponValue(TAG_Attribute_AttackSpeed) * GetAttributeValue(TAG_Attribute_AttackSpeed);
	ProjectileCount = GetWeaponValue(TAG_Attribute_AttackProjectiles) * GetAttributeValue(TAG_Attribute_AttackProjectiles);
	AttackInterval = ProjectileCount > 1 ? Interval * 0.5f / (ProjectileCount - 1) : 0.0f;
	ShootRandomTarget();
}

void USA_RangeAttack::ShootRandomTarget()
{
	if (ProjectileCount <= 0)
		return;

	if (Instigator.IsValid() == false || IsValid(ProjectileClass) == false || AttackTargets.IsEmpty())
		return;

	int RandomIdx = FMath::RandRange(0, AttackTargets.Num() - 1);
	if (AttackTargets[RandomIdx].IsValid() == false)
		return;

	FVector TargetDir = (AttackTargets[RandomIdx]->GetActorLocation() - Instigator->GetActorLocation()).GetSafeNormal();
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
			&ThisClass::ShootRandomTarget,
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
		float WeaponProjectileSpeed = GetWeaponValue(TAG_Attribute_ProjectileSpeed);
		float AttributeProjectileSpeed = GetAttributeValue(TAG_Attribute_ProjectileSpeed);
		float ProjectileSpeed = WeaponProjectileSpeed * AttributeProjectileSpeed;
		Projectile->SetDirectionAndSpeed(TargetDir, ProjectileSpeed);
		Projectile->SetTargetTag(TargetTag);
		float SkillDamage = GetWeaponValue(TAG_Attribute_Damage);
		float AttributeDamage = GetAttributeValue(TAG_Attribute_Damage);
		float Damage = SkillDamage * AttributeDamage;
		Projectile->SetDamage(Damage);
	}
}
