#include "Skill/WSA_AreaAttack.h"
#include "Engine/OverlapResult.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MegaBonkLike.h"

void UWSA_AreaAttack::Activate(TWeakObjectPtr<AActor> InInstigator)
{
	Super::Activate(InInstigator);

    TimerDelegate.BindUObject(this, &ThisClass::CheckHit);
    SetIntervalTimer();
}

void UWSA_AreaAttack::CheckHit()
{
    if (Instigator.IsValid() == false)
        return;

    FVector Origin = Instigator->GetActorLocation();
    float Radius = GetValue(TAG_Attribute_Size);
    TArray<FOverlapResult> Overlaps;
    FCollisionShape CollisionShape;
    CollisionShape.SetSphere(Radius);
    bool bHit = Instigator->GetWorld()->OverlapMultiByObjectType(
        Overlaps,
        Origin,
        FQuat::Identity,
        ECC_MBL_ENEMY,
        CollisionShape
    );

    if (bHit)
    {
        for (auto& Result : Overlaps)
        {
            AActor* HitActor = Result.GetActor();
            if (IsValid(HitActor) == false || HitActor == Instigator)
                continue;

            float Damage = GetValue(TAG_Attribute_Damage);
            UGameplayStatics::ApplyDamage(HitActor, Damage, Instigator->GetInstigatorController(), Instigator.Get(), nullptr);
        }
    }
}
