#include "Skill/SA_AttachToPlayer.h"
#include "Skill/DamageAreaActor.h"
#include "Kismet/GameplayStatics.h"
#include "Item/WeaponItem.h"
#include "Character/MBLPlayerCharacter.h"

void USA_AttachToPlayer::Activate(TWeakObjectPtr<AActor> InInstigator)
{
    Super::Activate(InInstigator);

    if (Instigator.IsValid() == true && IsValid(AttachedActorClass) == true)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = Instigator.Get();
        SpawnParams.Instigator = Cast<APawn>(Instigator);
        FVector SpawnLocation = Instigator->GetActorLocation() + LocationOffset;
        FRotator SpawnRotation = Instigator->GetActorRotation() + RotationOffset;
        AttachedActor = Instigator->GetWorld()->SpawnActor<ADamageAreaActor>(AttachedActorClass, SpawnLocation, SpawnRotation, SpawnParams);
        if (AttachedActor)
        {
            FAttachmentTransformRules AttachRules(EAttachmentRule::KeepWorld, true);
            AttachedActor->AttachToComponent(Instigator->GetRootComponent(), AttachRules);

            SetDamage();
            SetSize();
        }
    }

    OwnerWeapon->AddAttributeChangedCallback(TAG_Attribute_Damage, this, [this](const TWeakObjectPtr<UAttribute>) { SetDamage(); });
    OwnerWeapon->AddAttributeChangedCallback(TAG_Attribute_Size, this, [this](const TWeakObjectPtr<UAttribute>) { SetSize(); });
    if (AMBLPlayerCharacter* Character = Cast<AMBLPlayerCharacter>(Instigator))
    {
        Character->AddAttributeChangedCallback(TAG_Attribute_Damage, this, [this](const TWeakObjectPtr<UAttribute>) { SetDamage(); });
        Character->AddAttributeChangedCallback(TAG_Attribute_Size, this, [this](const TWeakObjectPtr<UAttribute>) { SetSize(); });
    }
    
    TimerDelegate.BindUObject(this, &ThisClass::CheckHit);
    SetIntervalTimer();
}

void USA_AttachToPlayer::Deactivate()
{
    if (IsValid(AttachedActor) == true)
    {
        AttachedActor->Destroy();
    }

    OwnerWeapon->RemoveAttributeChangedCallback(TAG_Attribute_Damage, this);
    OwnerWeapon->RemoveAttributeChangedCallback(TAG_Attribute_Size, this);
    if (AMBLPlayerCharacter* Character = Cast<AMBLPlayerCharacter>(Instigator))
    {
        Character->RemoveAttributeChangedCallback(TAG_Attribute_Damage, this);
        Character->RemoveAttributeChangedCallback(TAG_Attribute_Size, this);
    }
}

void USA_AttachToPlayer::SetDamage()
{
    if (IsValid(AttachedActor) == false)
        return;
    float Damage = GetWeaponValue(TAG_Attribute_Damage) * GetAttributeValue(TAG_Attribute_Damage);
    AttachedActor->SetDamage(Damage);
}

void USA_AttachToPlayer::SetSize()
{
    if (IsValid(AttachedActor) == false)
        return;
    float Size = GetWeaponValue(TAG_Attribute_Size) * GetAttributeValue(TAG_Attribute_Size);
    AttachedActor->SetActorScale3D(Size * FVector::OneVector);
}

void USA_AttachToPlayer::CheckHit()
{
    if (IsValid(AttachedActor) == false)
        return;

    AttachedActor->CheckHit();
}
