#include "Skill/SA_UnderFootAttack.h"
#include "Skill/DamageAreaActor.h"
#include "Character/MBLPlayerCharacter.h"
#include "Engine/HitResult.h"
#include "MegaBonkLike.h"

void USA_UnderFootAttack::Activate(TWeakObjectPtr<AActor> InInstigator)
{
	Super::Activate(InInstigator);

    TimerDelegate.BindUObject(this, &ThisClass::SpawnUnderFootAttackActor);
    SetIntervalTimer();
}

void USA_UnderFootAttack::SpawnUnderFootAttackActor()
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
            FVector SpawnLocation = Hit.Location + FVector::UpVector;
            FVector NormalVector = Hit.ImpactNormal;
            FRotator SpawnRotation = FRotationMatrix::MakeFromZ(NormalVector).Rotator();
            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = Character;
            SpawnParams.Instigator = Character;
            ADamageAreaActor* SpawnActor = Instigator->GetWorld()->SpawnActor<ADamageAreaActor>(
                SpawnActorClass, 
                SpawnLocation,
                SpawnRotation, 
                SpawnParams);

            if (SpawnActor)
            {
                float Damage = GetWeaponValue(TAG_Attribute_Damage) * GetAttributeValue(TAG_Attribute_Damage);
                float Size = GetWeaponValue(TAG_Attribute_Size) * GetAttributeValue(TAG_Attribute_Size);
                float LifeTime = GetWeaponValue(TAG_Attribute_Duration) * GetAttributeValue(TAG_Attribute_Duration);
                SpawnActor->SetDamage(Damage);
                SpawnActor->SetSize(Size);
                SpawnActor->SetLifeTime(LifeTime);
            }
        }
    }
}