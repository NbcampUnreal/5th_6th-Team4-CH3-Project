#include "Skill/SkillEffect/SE_Niagara.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

void USE_Niagara::Activate(const FVector& Location, const FRotator& Rotation)
{
    if (Owner.IsValid() == false || IsValid(NiagaraSystem) == false)
        return;

    if (bAttachToOwner == true)
    {
        UNiagaraFunctionLibrary::SpawnSystemAttached(
            NiagaraSystem,
            Owner->GetRootComponent(),
            NAME_None,
            Location,
            Rotation,
            EAttachLocation::KeepRelativeOffset,
            true);
    }
    else
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            Owner->GetWorld(),
            NiagaraSystem,
            Location,
            Rotation);
    }
}
