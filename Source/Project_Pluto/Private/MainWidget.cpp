// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "BossStateWidget.h"
#include "Components/ProgressBar.h"

void UMainWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetMaxHP(0);
	SetNowHP(0);
	ShowBossState(false);
}

void UMainWidget::SetNowHP(int32 NowHP)
{
	NowHPText->SetText(FText::AsNumber(NowHP));
}

void UMainWidget::SetMaxHP(int32 MaxHP)
{
	MaxHPText->SetText(FText::AsNumber(MaxHP));
}

void UMainWidget::SetHP(int32 Cur, int32 Max)
{
	if(HPBar == nullptr || HPInnerBar == nullptr) return;

	float HPPercent = Cur == 0 ? 0.0f : (float) Cur / (float) Max;
	
	HPBar->SetPercent(HPPercent);
	HPInnerBar->SetPercent((HPPercent - 0.01) > 0 ? (HPPercent - 0.01) : 0.0f);
}

void UMainWidget::ShowBossState(bool bShow)
{
	if (BossStateWidget == nullptr) return;

	BossStateWidget->SetVisibility(bShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}
