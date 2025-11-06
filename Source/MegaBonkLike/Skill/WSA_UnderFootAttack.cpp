#include "Skill/WSA_UnderFootAttack.h"
#include "Skill/DamageAreaActor.h"
#include "Character/MBLPlayerCharacter.h"
#include "Engine/HitResult.h"
#include "MegaBonkLike.h"
#include "Common/PoolSubsystem.h"

void UWSA_UnderFootAttack::Activate(TWeakObjectPtr<AActor> InInstigator)
{
	Super::Activate(InInstigator);

    TimerDelegate.BindUObject(this, &ThisClass::SpawnUnderFootAttackActor);
    SetIntervalTimer();
}

void UWSA_UnderFootAttack::SpawnUnderFootAttackActor()
{
	AMBLPlayerCharacter* Character = Cast<AMBLPlayerCharacter>(Instigator);
    if (IsValid(Character) == true && IsValid(SpawnActorClass) == true)
    {
        FHitResult Hit;
        FVector StartLocation = Character->GetFootLocation();
        FVector EndLocation = StartLocation + FVector(0.0f, 0.0f, -5000.0f);
        FCollisionObjectQueryParams ObjectParams;
        ObjectParams.AddObjectTypesToQuery(ECC_WorldStatic);
        if (Character->GetWorld()->LineTraceSingleByObjectType(Hit, StartLocation, EndLocation, ObjectParams))
        {
            UPoolSubsystem* PoolSubsystem = GetWorld()->GetSubsystem<UPoolSubsystem>();
            if (IsValid(PoolSubsystem) == false)
                return;

            FVector SpawnLocation = Hit.Location + FVector::UpVector;
            FVector NormalVector = Hit.ImpactNormal;
            FRotator SpawnRotation = FRotationMatrix::MakeFromZ(NormalVector).Rotator();
            ADamageAreaActor* SpawnActor = PoolSubsystem->GetFromPool<ADamageAreaActor>(
                SpawnActorClass, 
                SpawnLocation,
                SpawnRotation);

            if (IsValid(SpawnActor) == true)
            {
                SpawnActor->SetOwner(Character);
                SpawnActor->SetInstigator(Character);

                float Size = GetValue(TAG_Attribute_Size);
                float LifeTime = GetValue(TAG_Attribute_Duration);
                SpawnActor->SetAttackData(CreateAttackDataBase());
                SpawnActor->SetSize(Size);
                SpawnActor->SetLifeTime(LifeTime);
                SpawnActor->SetHitTimer(LifeTime / GetValue(TAG_Attribute_AttackProjectiles));
            }
        }
    }
}