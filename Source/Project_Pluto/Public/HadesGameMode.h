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
	//사운드 추가를 위한 생성자 만들기
	AHadesGameMode();

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

	int32 NowHP = 0;

public:
	// 저장하기 / 불러오기
	// HPSave 갱신될 때 저장
	// 다음 맵 갈 때만 HPSave 불러오기

	// SlotName, UserIndex
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SaveSlotName = TEXT("HPSave");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 UserIndex = 0;

	void SaveGameData();
	void LoadGameData();

	UPROPERTY()
	class USoundBase* GameSound;
	int32 beforeMusic = 0;
};
