#include "Character/EnemyBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/MonsterStat.h"

AEnemyBase::AEnemyBase()
{
}
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	if (GetMesh())
	{
		int32 MaterialCount = GetMesh()->GetNumMaterials();
		for (int32 i = 0; i < MaterialCount; i++)
		{
			UMaterialInterface* Mat = GetMesh()->GetMaterial(i);
			if (Mat)
			{
				UMaterialInstanceDynamic* DynMat = UMaterialInstanceDynamic::Create(Mat, this);
				GetMesh()->SetMaterial(i, DynMat);
				DynamicMaterials.Add(DynMat);
			}
		}
	}
}

void AEnemyBase::SetAttack(EMBLWaveState Wave)
{
	if (!IsValid(StatDataTable)) return;

	if (EMBLWaveState::SetWave < Wave)
	{
		if (Wave > EMBLWaveState::Wave3)
		{
			Wave = EMBLWaveState::Wave3;
		}

		FName RowName(*StaticEnum<EMBLWaveState>()->GetNameStringByValue((int64)Wave));
		FMonsterStat* Monster = StatDataTable->FindRow<FMonsterStat>(RowName, TEXT(""));

	

		if (!Monster) return;

		Attack = Monster->Attack;
	}
}

void AEnemyBase::SetSpeed(EMBLWaveState Wave)
{
	if (!IsValid(StatDataTable)) return;

	if (EMBLWaveState::SetWave < Wave)
	{
		if (Wave > EMBLWaveState::Wave3)
		{
			Wave = EMBLWaveState::Wave3;
		}

		FName RowName(*StaticEnum<EMBLWaveState>()->GetNameStringByValue((int64)Wave));
		FMonsterStat* Monster = StatDataTable->FindRow<FMonsterStat>(RowName, TEXT(""));

		

		if (!Monster) return;

		GetCharacterMovement()->MaxWalkSpeed = Monster->MoveSpeed;
	}
}

void AEnemyBase::SetColor(EMBLWaveState Wave)
{
	if (!IsValid(StatDataTable)) return;

	if (EMBLWaveState::SetWave < Wave )
	{
		if (Wave > EMBLWaveState::Wave3)
		{
			Wave = EMBLWaveState::Wave3;
		}

		//DynamicMaterials.Empty();
		FName RowName(*StaticEnum<EMBLWaveState>()->GetNameStringByValue((int64)Wave));
 		FMonsterStat* Monster = StatDataTable->FindRow<FMonsterStat>(RowName, TEXT(""));

		if (!Monster) return;
		
		for (UMaterialInstanceDynamic* DynMat : DynamicMaterials)
		{
			if (DynMat)
			{
				DynMat->SetVectorParameterValue(FName("BaseColor"), Monster->BodyColor);
			}
		}
	}
}

void AEnemyBase::SetMaxHealth(EMBLWaveState Wave)
{
	if (!IsValid(StatDataTable)) return;

	if (EMBLWaveState::SetWave < Wave)
	{
		if (Wave > EMBLWaveState::Wave3)
		{
			Wave = EMBLWaveState::Wave3;
		}

		FName RowName(*StaticEnum<EMBLWaveState>()->GetNameStringByValue((int64)Wave));
		FMonsterStat* Monster = StatDataTable->FindRow<FMonsterStat>(RowName, TEXT(""));

		if (!Monster) return;

		MaxHP = Monster->MaxHealth;
		CurrHP = MaxHP;
	}
}

void AEnemyBase::SetAll(EMBLWaveState Wave)
{
	SetAttack(Wave);
	SetSpeed(Wave);
	SetColor(Wave);
	SetMaxHealth(Wave);
}



