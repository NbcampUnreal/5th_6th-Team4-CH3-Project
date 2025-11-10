#include "MonsterAttack/MonsterWeapon/EliteWeapon.h"

AEliteWeapon::AEliteWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	SetRootComponent(StaticMeshComponent);

}

void AEliteWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

