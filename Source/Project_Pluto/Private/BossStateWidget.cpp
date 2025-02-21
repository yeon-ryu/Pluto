// Fill out your copyright notice in the Description page of Project Settings.


#include "BossStateWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UBossStateWidget::NativeConstruct()
{
	SetName(TEXT("Megaera"));
	SetHP(0, 0);
}

void UBossStateWidget::SetName(FString name)
{
	NameText->SetText(FText::FromString(name));
}

void UBossStateWidget::SetHP(int32 Cur, int32 Max)
{
	if (HPBar == nullptr) return;

	float HPPercent = Cur == 0 ? 0.0f : (float)Cur / (float)Max;

	HPBar->SetPercent(HPPercent);
}
