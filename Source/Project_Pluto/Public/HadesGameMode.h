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
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UMainWidget> mainWidget;

	class UMainWidget* mainUI;

	void SetPlayerHP(int32 Cur, int32 Max);
	void SetBossHP(int32 Cur, int32 Max);
	void SetBossName(FString name);
	void ShowBossState(bool bShow);
	void ShowGameOver(bool bShow);
	void ShowGameClear(bool bShow);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UGameOverWidget> gameOverWidget;

	class UGameOverWidget* gameOverUI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UGameClearWidget> gameClearWidget;

	class UGameClearWidget* gameClearUI;

	int32 HPBuff = 0;

public:
	// 저장하기 / 불러오기
	// HPBuff 갱신될 때 저장
	// 다음 맵 갈 때만 HPBuff 불러오기

	// SlotName, UserIndex
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SaveSlotName = TEXT("HPBuff");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 UserIndex = 0;

	void SaveGameData(int32 buff);
	void LoadGameData();
};
