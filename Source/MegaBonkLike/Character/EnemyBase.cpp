#include "Character/EnemyBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/MonsterStat.h"

AEnemyBase::AEnemyBase()
{
}
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();    //이거 왜 넣고 안넣고 차이가? 

	//SetAttack(CurrentWave);
	//SetSpeed(CurrentWave);
	//SetColor(CurrentWave);

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

	if (EMBLWaveState::SetWave < Wave && Wave < EMBLWaveState::FinalWave)
	{
		FName RowName(*StaticEnum<EMBLWaveState>()->GetNameStringByValue((int64)Wave));
		FMonsterStat* Monster = StatDataTable->FindRow<FMonsterStat>(RowName, TEXT(""));

		if (!Monster) return;

		Attack = Monster->Attack;
	}
}

void AEnemyBase::SetSpeed(EMBLWaveState Wave)
{
	if (!IsValid(StatDataTable)) return;

	if (EMBLWaveState::SetWave < Wave && Wave < EMBLWaveState::FinalWave)
	{
		FName RowName(*StaticEnum<EMBLWaveState>()->GetNameStringByValue((int64)Wave));
		FMonsterStat* Monster = StatDataTable->FindRow<FMonsterStat>(RowName, TEXT(""));

		if (!Monster) return;

		GetCharacterMovement()->MaxWalkSpeed = Monster->MoveSpeed;
	}
}

void AEnemyBase::SetColor(EMBLWaveState Wave)
{
	if (!IsValid(StatDataTable)) return;

	if (EMBLWaveState::SetWave < Wave && Wave < EMBLWaveState::FinalWave)
	{
		//DynamicMaterials.Empty();

		//if (GetMesh())
		//{
		//	int32 MaterialCount = GetMesh()->GetNumMaterials();
		//	for (int32 i = 0; i < MaterialCount; i++)
		//	{
		//		UMaterialInterface* Mat = GetMesh()->GetMaterial(i);
		//		if (Mat)
		//		{
		//			UMaterialInstanceDynamic* DynMat = UMaterialInstanceDynamic::Create(Mat, this);
		//			GetMesh()->SetMaterial(i, DynMat);
		//			DynamicMaterials.Add(DynMat);
		//		}
		//	}
		//}

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



