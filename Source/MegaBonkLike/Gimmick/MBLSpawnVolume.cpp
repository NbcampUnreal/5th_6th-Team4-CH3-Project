// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/MBLSpawnVolume.h"

// Sets default values
AMBLSpawnVolume::AMBLSpawnVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMBLSpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMBLSpawnVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

