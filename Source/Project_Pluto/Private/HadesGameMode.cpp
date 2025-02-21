// Fill out your copyright notice in the Description page of Project Settings.


#include "HadesGameMode.h"
#include "MainWidget.h"
#include "BossStateWidget.h"
#include "GameOverWidget.h"

void AHadesGameMode::BeginPlay()
{
	Super::BeginPlay();

	auto controller = GetWorld()->GetFirstPlayerController();
	
	if(controller == nullptr) return;

	// 마우스 커서 설정
	controller->bShowMouseCursor = true;

	// 마우스 디자인 Project Settings 에서 Default 에 설정한거 적용
	controller->DefaultMouseCursor = EMouseCursor::Default;
	controller->CurrentMouseCursor = EMouseCursor::Default;

	if (mainWidget != nullptr) {
		// mainWidget 블루프린트 파일을 메모리에 로드
		mainUI = CreateWidget<UMainWidget>(GetWorld(), mainWidget);

		// 위젯이 메모리에 로드되면 뷰 포트에 출력
		if (mainUI != nullptr) {
			mainUI->AddToViewport();

			// 기본적으로 input focus 게임으로 적용 -> 공격 씹힘
			//controller->SetInputMode(FInputModeGameOnly());
		}
	}

	if (gameOverWidget != nullptr) {
		// gameOverWidget 블루프린트 파일을 메모리에 로드
		gameOverUI = CreateWidget<UGameOverWidget>(GetWorld(), gameOverWidget);
	}
}

void AHadesGameMode::SetPlayerHP(int32 Cur, int32 Max)
{
	if(mainUI == nullptr) return;
	mainUI->SetHP(Cur, Max);
	
	mainUI->SetNowHP(Cur);
	mainUI->SetMaxHP(Max);
}

void AHadesGameMode::SetBossHP(int32 Cur, int32 Max)
{
	if(mainUI == nullptr || mainUI->BossStateWidget == nullptr) return;
	mainUI->BossStateWidget->SetHP(Cur, Max);
}

void AHadesGameMode::ShowBossState(bool bShow)
{
	if(mainUI == nullptr) return;

	mainUI->ShowBossState(bShow);
}

void AHadesGameMode::ShowGameOver(bool bShow)
{
	//if (mainUI == nullptr) return;

	////auto controller = GetWorld()->GetFirstPlayerController();
	////if(controller == nullptr) return;
	////controller->PlayerCameraManager->StartCameraFade(1.f, 0.f, 1.f, FLinearColor::Black, true, true);

	//if (bShow) {
	//	if (gameOverUI != nullptr) {
	//		gameOverUI->AddToViewport();
	//		//controller->SetInputMode(FInputModeUIOnly());
	//	}
	//}
	//else {
	//	if (gameOverUI != nullptr) {
	//		gameOverUI->RemoveFromParent();
	//		//controller->SetInputMode(FInputModeGameOnly());
	//	}
	//}
}
