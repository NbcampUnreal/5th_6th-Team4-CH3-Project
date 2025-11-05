#include "Skill/WSA_AttachToPlayer.h"
#include "Skill/DamageAreaActor.h"
#include "Kismet/GameplayStatics.h"
#include "Item/WeaponItem.h"
#include "Character/MBLPlayerCharacter.h"

void UWSA_AttachToPlayer::Activate(TWeakObjectPtr<AActor> InInstigator)
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

            SetAttackData();
            SetSize();
        }
    }

    OwnerWeapon->AddAttributeChangedCallback(TAG_Attribute_Damage, this, [this](const TWeakObjectPtr<UAttribute>) { SetAttackData(); });
    OwnerWeapon->AddAttributeChangedCallback(TAG_Attribute_Size, this, [this](const TWeakObjectPtr<UAttribute>) { SetSize(); });
    if (AMBLPlayerCharacter* Character = Cast<AMBLPlayerCharacter>(Instigator))
    {
        Character->AddAttributeChangedCallback(TAG_Attribute_Damage, this, [this](const TWeakObjectPtr<UAttribute>) { SetAttackData(); });
        Character->AddAttributeChangedCallback(TAG_Attribute_Size, this, [this](const TWeakObjectPtr<UAttribute>) { SetSize(); });
    }
    
    TimerDelegate.BindUObject(this, &ThisClass::CheckHit);
    SetIntervalTimer();

    CheckHitEffects.SetOwner(Instigator);
}

void UWSA_AttachToPlayer::Deactivate()
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

void UWSA_AttachToPlayer::SetAttackData()
{
    if (IsValid(AttachedActor) == false)
        return;

    AttachedActor->SetAttackData(CreateAttackDataBase());
}

void UWSA_AttachToPlayer::SetSize()
{
    if (IsValid(AttachedActor) == false)
        return;

    AttachedActor->SetActorScale3D(GetValue(TAG_Attribute_Size) * FVector::OneVector);
}

void UWSA_AttachToPlayer::CheckHit()
{
    if (IsValid(AttachedActor) == false)
        return;

    AttachedActor->CheckHitOnNextFrame();
    CheckHitEffects.ActivateAll(FVector::ZeroVector, FRotator::ZeroRotator);
}
