// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BossStateWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_PLUTO_API UBossStateWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* NameText;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UProgressBar* HPBar;
	
	void SetName(FString name);
	void SetHP(int32 Cur, int32 Max);
};
