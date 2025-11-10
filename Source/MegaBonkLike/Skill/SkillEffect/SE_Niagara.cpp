#include "Skill/SkillEffect/SE_Niagara.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

const FName USE_Niagara::NameScale = FName(TEXT("User.Scale"));

void USE_Niagara::Activate(const FVector& Location, const FRotator& Rotation, float InScale)
{
    if (Owner.IsValid() == false || IsValid(NiagaraSystem) == false)
        return;

    // scale 너무 작으면 이상함
    if (InScale < 0.1f)
        return;

    UNiagaraComponent* Comp = nullptr;
    if (bAttachToOwner == true)
    {
        Comp = UNiagaraFunctionLibrary::SpawnSystemAttached(
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
        Comp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            Owner->GetWorld(),
            NiagaraSystem,
            Location,
            Rotation);
    }

    if (IsValid(Comp) == true)
    {
        Comp->SetVariableFloat(NameScale, InScale);
    }
}
