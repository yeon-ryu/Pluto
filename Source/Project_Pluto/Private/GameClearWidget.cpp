// Fill out your copyright notice in the Description page of Project Settings.


#include "GameClearWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UGameClearWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 각 버튼 입력 이벤트에 대응하는 함수를 연결
	Button_Restart->OnClicked.AddDynamic(this, &UGameClearWidget::Restart);
	Button_Quit->OnClicked.AddDynamic(this, &UGameClearWidget::Quit);
}

void UGameClearWidget::Restart()
{
	// 게임 클리어시 최초 레벨을 로드
	UGameplayStatics::OpenLevel(GetWorld(), FName("HadesMap1"));
}

void UGameClearWidget::Quit()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, false);
}
