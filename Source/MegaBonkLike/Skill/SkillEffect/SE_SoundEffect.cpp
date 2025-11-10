#include "Skill/SkillEffect/SE_SoundEffect.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

void USE_SoundEffect::Activate(const FVector& Location, const FRotator& Rotation, float InScale)
{
    if (Owner.IsValid() == false || IsValid(Sound) == false)
        return;

    if (IsValid(Audio) == false)
    {
        Audio = UGameplayStatics::SpawnSoundAttached(Sound, Owner->GetRootComponent());
    }

    Audio->VolumeMultiplier = InScale;
    Audio->Play();
}

void USE_SoundEffect::Deactivate()
{
    if (IsValid(Audio) == false)
        return;

    Audio->Stop();
}
