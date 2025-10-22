#include "Skill/SA_AttachToPlayer.h"
#include "Kismet/GameplayStatics.h"

USA_AttachToPlayer::USA_AttachToPlayer()
{
    bUseTick = true;
}

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
        AttachedActor = Instigator->GetWorld()->SpawnActor<AActor>(AttachedActorClass, SpawnLocation, SpawnRotation, SpawnParams);

        TArray<UPrimitiveComponent*> PrimitiveComponents;
        AttachedActor->GetComponents<UPrimitiveComponent>(PrimitiveComponents);
        for (auto* PrimComp : PrimitiveComponents)
        {
            PrimComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
            PrimComp->SetGenerateOverlapEvents(true);
            PrimComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
            PrimComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
        }
    }

    if (bUseCollisionHit == true)
    {
        TimerDelegate.BindUObject(this, &ThisClass::CheckHit);
        SetIntervalTimer();
    }
}

void USA_AttachToPlayer::Tick(float DeltaTime)
{
    UpdateTransform();
}

void USA_AttachToPlayer::Deactivate()
{
    if (IsValid(AttachedActor) == true)
    {
        AttachedActor->Destroy();
    }
}

void USA_AttachToPlayer::CheckHit()
{
    TArray<UPrimitiveComponent*> PrimitiveComponents;
    AttachedActor->GetComponents<UPrimitiveComponent>(PrimitiveComponents);
    for (UPrimitiveComponent* PrimComp : PrimitiveComponents)
    {
        if (IsValid(PrimComp) == false || PrimComp->GetGenerateOverlapEvents() == false)
            return;

        TArray<AActor*> OverlappingActors;
        PrimComp->GetOverlappingActors(OverlappingActors);

        for (AActor* OverlapActor : OverlappingActors)
        {
            // 액터가 유효하지 않으면
            if (IsValid(OverlapActor) == false || OverlapActor == Instigator)
                continue;

            // 태그 대상이 아니라면
            if (TargetTag.IsNone() == false && OverlapActor->ActorHasTag(TargetTag) == false)
                continue;

            ApplyDamage(OverlapActor);
        }
    }
}

void USA_AttachToPlayer::UpdateTransform()
{
    if (IsValid(AttachedActor) == false || Instigator.IsValid() == false)
        return;

    AttachedActor->SetActorLocation(Instigator->GetActorLocation() + LocationOffset);
    AttachedActor->SetActorRotation(AttachedActor->GetActorRotation() + RotationOffset);
    SetSize();
}

// 이거 delegate에 연결하기
void USA_AttachToPlayer::SetSize()
{
    float SkillSize = GetSkillValue(BaseSize, TAG_Attribute_Size);
    float AttributeSize = GetAttributeValue(TAG_Attribute_Size);
    float Size = SkillSize * AttributeSize;
    AttachedActor->SetActorScale3D(Size * FVector::OneVector);
}

void USA_AttachToPlayer::ApplyDamage(AActor* TargetActor)
{
    if (Instigator.IsValid() == false)
        return;

    float SkillDamage = GetSkillValue(BaseDamage, TAG_Attribute_Damage);
    float AttributeDamage = GetAttributeValue(TAG_Attribute_Damage);
    float Damage = SkillDamage * AttributeDamage;
    UGameplayStatics::ApplyDamage(TargetActor, Damage, Instigator->GetInstigatorController(), Instigator.Get(), nullptr);
}
