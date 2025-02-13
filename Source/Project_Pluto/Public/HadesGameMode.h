// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HadesGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_PLUTO_API AHadesGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void StartPlay();

	// 마우스 커서 디자인
	UPROPERTY(EditDefaultsOnly, Category=SniperUI)
	TSubclassOf<class UUserWidget> MouseCursorFactory;

	UPROPERTY()
	class UUserWidget* MouseCursor;
};
