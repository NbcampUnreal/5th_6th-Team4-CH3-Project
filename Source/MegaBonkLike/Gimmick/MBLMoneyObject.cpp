// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/MBLMoneyObject.h"

AMBLMoneyObject::AMBLMoneyObject()
	: MoneyValue(5)
{
	SpawnObjectType = "Money";
}

void AMBLMoneyObject::OnObjectActivated(AActor* Activator)
{
	Super::OnObjectActivated(Activator);
	Super::DestroyObject();
}
