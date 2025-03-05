// Fill out your copyright notice in the Description page of Project Settings.


#include "HadesGameMode.h"
#include "MainWidget.h"
#include "BossStateWidget.h"
#include "GameOverWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameClearWidget.h"
#include "HadesSaveGame.h"


//생성자 파트는 전부 사운드 추가 파트입니다.
AHadesGameMode::AHadesGameMode()
{

	//노래 이것저것 넣어두고 랜덤재생 시킴
	//잔잔한 노래라... 게임 오버에나 맞을듯... 시프트에서 빼버렸습니다.
	//ConstructorHelpers::FObjectFinder<USoundBase> tmpSound_0(TEXT("/Script/Engine.SoundWave'/Game/KJY/RSS/God_of_the_Dead_-//_Hades_mp3.God_of_the_Dead_-_Hades_mp3'"));
	

	//1-3번만 랜덤에 포함시킴
	ConstructorHelpers::FObjectFinder<USoundBase> tmpSound_1(TEXT("/Script/Engine.SoundWave'/Game/KJY/RSS/Hades_-_The_Painful_Way.Hades_-_The_Painful_Way'"));

	ConstructorHelpers::FObjectFinder<USoundBase> tmpSound_2(TEXT("/Script/Engine.SoundWave'/Game/KJY/RSS/Mouth_of_Styx_-_Hades_OST.Mouth_of_Styx_-_Hades_OST'"));

	ConstructorHelpers::FObjectFinder<USoundBase> tmpSound_3(TEXT("/Script/Engine.SoundWave'/Game/KJY/RSS/Out_of_Tartarus_-_Hades_OST.Out_of_Tartarus_-_Hades_OST'"));

	int32 randMusic = FMath::RandRange(1,3);

	while (randMusic == beforeMusic)
	{
		randMusic = FMath::RandRange(1, 3);
	}

		switch (randMusic)
		{
		case 1:
			GameSound = tmpSound_1.Object;
			beforeMusic = randMusic;
			break;

		case 2:
			GameSound = tmpSound_2.Object;
			beforeMusic = randMusic;
			break;

		case 3:
			GameSound = tmpSound_3.Object;
			beforeMusic = randMusic;
			break;
		}


}



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

	LoadGameData();

	if (mainWidget != nullptr) {
		// mainWidget 블루프린트 파일을 메모리에 로드
		mainUI = CreateWidget<UMainWidget>(GetWorld(), mainWidget);

		// 위젯이 메모리에 로드되면 뷰 포트에 출력
		if (mainUI != nullptr) {
			mainUI->AddToViewport();

			// 기본적으로 input focus 게임으로 적용 -> 공격 씹힘
			//controller->SetInputMode(FInputModeGameOnly());

			if (mainUI->BossStateWidget != nullptr) {
				mainUI->BossStateWidget->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}

	if (gameOverWidget != nullptr) {
		// gameOverWidget 블루프린트 파일을 메모리에 로드
		gameOverUI = CreateWidget<UGameOverWidget>(GetWorld(), gameOverWidget);
		//gameOverUI->AddToViewport();
		//gameOverUI->SetVisibility(ESlateVisibility::Hidden);
	}

	if (gameClearWidget != nullptr) {
		// gameClearWidget 블루프린트 파일을 메모리에 로드
		gameClearUI = CreateWidget<UGameClearWidget>(GetWorld(), gameClearWidget);
		//gameClearUI->AddToViewport();
		//gameClearUI->SetVisibility(ESlateVisibility::Hidden);
	}


	//사운드 플레이 추가
	UGameplayStatics::PlaySound2D(GetWorld(), GameSound);
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

void AHadesGameMode::SetBossName(FString name)
{
	if (mainUI == nullptr || mainUI->BossStateWidget == nullptr) return;
	mainUI->BossStateWidget->SetName(name);
}

void AHadesGameMode::ShowBossState(bool bShow)
{
	if(mainUI == nullptr) return;

	mainUI->ShowBossState(bShow);
}

void AHadesGameMode::ShowGameOver(bool bShow)
{
	if (mainUI == nullptr) return;

	UGameplayStatics::SetGamePaused(GetWorld(), bShow);
	mainUI->ShowGameOver(bShow);

	if(bShow) {
		HPBuff = 0;
		NowHP = 0;
		SaveGameData();
	}
}

void AHadesGameMode::ShowGameClear(bool bShow)
{
	if (mainUI == nullptr) return;

	UGameplayStatics::SetGamePaused(GetWorld(), bShow);
	mainUI->ShowGameClear(bShow);

	if (bShow) {
		HPBuff = 0;
		NowHP = 0;
		SaveGameData();
	}
}

void AHadesGameMode::SaveGameData()
{
	// SaveGame 객체를 생성
	UHadesSaveGame* sg = Cast<UHadesSaveGame>(UGameplayStatics::CreateSaveGameObject(UHadesSaveGame::StaticClass()));

	// 객체에 갱신
	sg->HPBuffSave = HPBuff;
	sg->nowHPSave = NowHP;

	// 파일로 저장
	UGameplayStatics::SaveGameToSlot(sg, SaveSlotName, UserIndex);


	UE_LOG(LogTemp, Error, TEXT("Save Game : Buff : %d, NowHP : %d"), HPBuff, NowHP);
}

void AHadesGameMode::LoadGameData()
{
	// 파일이 존재하는지 확인
	bool isExist = UGameplayStatics::DoesSaveGameExist(SaveSlotName, UserIndex);
	if (isExist == false) return;

	// 파일이 있으면 읽어서 ShootingSaveGame 객체를 생성
	UHadesSaveGame* sg = Cast<UHadesSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, UserIndex));

	// 값을 읽어와서 HighScore 갱신
	if (sg != nullptr) {
		HPBuff = sg->HPBuffSave;
		NowHP = sg->nowHPSave;
	}
}
