// Fill out your copyright notice in the Description page of Project Settings.


#include "HadesGameMode.h"

void AHadesGameMode::StartPlay()
{
	Super::StartPlay();

	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
}
