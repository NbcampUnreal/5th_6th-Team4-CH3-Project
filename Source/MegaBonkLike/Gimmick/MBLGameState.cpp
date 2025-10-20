// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/MBLGameState.h"

AMBLGameState::AMBLGameState()
	: KillCount(0)
	, TimeAttack(0.f)
	, WaveDuration(0.f)
	, MaxWave(3)
	, CurrentWaveIndex(0)
{
}

void AMBLGameState::BeginPlay()
{
	Super::BeginPlay();
}
