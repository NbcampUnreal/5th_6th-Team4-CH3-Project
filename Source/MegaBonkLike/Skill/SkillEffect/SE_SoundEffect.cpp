#include "Skill/SkillEffect/SE_SoundEffect.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

void USE_SoundEffect::Activate(const FVector& Location, const FRotator& Rotation)
{
    if (Owner.IsValid() == false || IsValid(Sound) == false)
        return;

    Audio = UGameplayStatics::SpawnSoundAttached(Sound, Owner->GetRootComponent());
}

void USE_SoundEffect::Deactivate()
{
    if (IsValid(Audio) == false)
        return;

    Audio->Stop();
    Audio = nullptr;
}
