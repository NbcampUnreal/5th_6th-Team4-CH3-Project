#include "Character/EnemyBase.h"
#include "GameFramework/CharacterMovementComponent.h"

AEnemyBase::AEnemyBase()
{

}
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();    //이거 왜 넣고 안넣고 차이가? 

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

void AEnemyBase::SetAttack(float NewAttack)
{
	Attack = NewAttack;
	//FName RowName(*StaticEnum<EMBLWaveState>()->GetNameStringByValue((int64)Wave));
}

void AEnemyBase::SetSpeed(float NewSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
}

void AEnemyBase::SetColor(const FLinearColor& NewColor)
{
	for (UMaterialInstanceDynamic* DynMat : DynamicMaterials)
	{
		if (DynMat)
		{
			DynMat->SetVectorParameterValue(FName("BaseColor"), NewColor);
		}
	}
}


