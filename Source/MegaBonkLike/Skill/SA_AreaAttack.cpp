#include "Skill/SA_AreaAttack.h"
#include "Engine/OverlapResult.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void USA_AreaAttack::Activate(TWeakObjectPtr<AActor> InInstigator)
{
	Super::Activate(InInstigator);
    TimerDelegate.BindUObject(this, &ThisClass::CheckHit);

    SetIntervalTimer();
}

void USA_AreaAttack::CheckHit()
{
    if (Instigator.IsValid() == false)
        return;

    FVector Origin = Instigator->GetActorLocation();
    float SkillRadius = GetWeaponValue(TAG_Attribute_Size);
    float AttributeRadius = GetAttributeValue(TAG_Attribute_Size);
    float Radius = SkillRadius * AttributeRadius;
    TArray<FOverlapResult> Overlaps;
    FCollisionShape CollisionShape;
    CollisionShape.SetSphere(Radius);
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

            float SkillDamage = GetWeaponValue(TAG_Attribute_Damage);
            float AttributeDamage = GetAttributeValue(TAG_Attribute_Damage);
            float Damage = SkillDamage * AttributeDamage;
            UGameplayStatics::ApplyDamage(HitActor, Damage, Instigator->GetInstigatorController(), Instigator.Get(), nullptr);
        }
    }
}
