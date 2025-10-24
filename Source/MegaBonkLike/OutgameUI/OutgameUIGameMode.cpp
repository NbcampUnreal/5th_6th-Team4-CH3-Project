// Fill out your copyright notice in the Description page of Project Settings.


#include "OutgameUI/OutgameUIGameMode.h"
#include "OutgameUI/OutgameUIPlayerController.h"

AOutgameUIGameMode::AOutgameUIGameMode()
{
    PlayerControllerClass = AOutgameUIPlayerController::StaticClass();
}