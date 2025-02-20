// Fill out your copyright notice in the Description page of Project Settings.


#include "HadesGameMode.h"

void AHadesGameMode::StartPlay()
{
	Super::StartPlay();

	auto controller = GetWorld()->GetFirstPlayerController();
	
	if(controller == nullptr) return;

	// 마우스 커서 설정
	controller->bShowMouseCursor = true;

	// 마우스 디자인 Project Settings 에서 Default 에 설정한거 적용
	controller->DefaultMouseCursor = EMouseCursor::Default;
	controller->CurrentMouseCursor = EMouseCursor::Default;

	// 기본적으로 input focus 게임으로 적용
	FInputModeGameOnly inputModeGameOnly;
	controller->SetInputMode(inputModeGameOnly);
}
