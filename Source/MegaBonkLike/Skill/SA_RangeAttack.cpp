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

	FVector Origin = Instigator->GetActorLocation();
	TArray<FOverlapResult> Overlaps;
	FCollisionShape CollisionShape;
	CollisionShape.SetSphere(AutoDetectRadius);
	bool bHit = Instigator->GetWorld()->OverlapMultiByChannel(
		Overlaps,
		Origin,
		FQuat::Identity,
		ECC_Pawn,
		CollisionShape
	);

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

	float Interval = BaseTimerInterval / GetSkillValue(1.0f, TAG_Attribute_AttackSpeed) * GetAttributeValue(TAG_Attribute_AttackSpeed);
	AttackCount = GetSkillValue(BaseProjectileCount, TAG_Attribute_AttackProjectiles) * GetAttributeValue(TAG_Attribute_AttackProjectiles);
	AttackInterval = AttackCount > 1 ? Interval * 0.5f / (AttackCount - 1) : 0.0f;
	ShootRandomTarget();
}

void USA_RangeAttack::ShootRandomTarget()
{
	if (AttackCount-- <= 0)
		return;

	if (Instigator.IsValid() == false || IsValid(ProjectileClass) == false || AttackTargets.IsEmpty())
		return;

	int RandomIdx = FMath::RandRange(0, AttackTargets.Num() - 1);
	if (AttackTargets[RandomIdx].IsValid() == false)
		return;

	Shoot(AttackTargets[RandomIdx]->GetActorLocation());

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

void USA_RangeAttack::Shoot(const FVector& TargetPos)
{
	if (Instigator.IsValid() == false)
		return;

	FVector SpawnLocation = Instigator->GetActorLocation() + Instigator->GetActorForwardVector() * 30.f;
	FVector Dir = (TargetPos - Instigator->GetActorLocation()).GetSafeNormal();
	FRotator SpawnRotation = Dir.Rotation();
	FActorSpawnParameters Params;
	Params.Owner = Instigator.Get();
	Params.Instigator = Cast<APawn>(Instigator.Get());
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AProjectile* Projectile = Instigator->GetWorld()->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, Params);
	if (IsValid(Projectile) == true)
	{
		float SkillProjectileSpeed = GetSkillValue(BaseProjectileSpeed, TAG_Attribute_ProjectileSpeed);
		float AttributeProjectileSpeed = GetAttributeValue(TAG_Attribute_ProjectileSpeed);
		float ProjectileSpeed = SkillProjectileSpeed * AttributeProjectileSpeed;
		Projectile->SetDirectionAndSpeed(Dir, ProjectileSpeed);
		Projectile->SetTargetTag(TargetTag);
		float SkillDamage = GetSkillValue(BaseDamage, TAG_Attribute_Damage);
		float AttributeDamage = GetAttributeValue(TAG_Attribute_Damage);
		float Damage = SkillDamage * AttributeDamage;
		Projectile->SetDamage(Damage);
	}
}
