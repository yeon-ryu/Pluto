// Fill out your copyright notice in the Description page of Project Settings.


#include "HadesGameMode.h"

void AHadesGameMode::StartPlay()
{
	Super::StartPlay();

	auto controller = GetWorld()->GetFirstPlayerController();
	
	if(controller == nullptr) return;

	controller->bShowMouseCursor = true;
	controller->DefaultMouseCursor = EMouseCursor::Default;
	controller->CurrentMouseCursor = EMouseCursor::Default;
}
