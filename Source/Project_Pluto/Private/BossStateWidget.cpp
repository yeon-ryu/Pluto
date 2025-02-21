// Fill out your copyright notice in the Description page of Project Settings.


#include "BossStateWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UBossStateWidget::SetName(FString name)
{
	if (name.IsEmpty())
	{
		FString txt = TEXT("Megaera");
		NameText->SetText(FText::FromString(txt));
	}
	NameText->SetText(FText::FromString(name));
}

void UBossStateWidget::SetHP(int32 Cur, int32 Max)
{
	if (HPBar == nullptr) return;

	float HPPercent = Cur == 0 ? 0.0f : (float)Cur / (float)Max;

	HPBar->SetPercent(HPPercent);
}
