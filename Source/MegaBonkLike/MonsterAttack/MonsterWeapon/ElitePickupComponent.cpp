#include "MonsterAttack/MonsterWeapon/ElitePickupComponent.h"
#include "ElitePickupComponent.h"

UElitePickupComponent::UElitePickupComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UElitePickupComponent::BeginPlay()
{
	Super::BeginPlay();
}
