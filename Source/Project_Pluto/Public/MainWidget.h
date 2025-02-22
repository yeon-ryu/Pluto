// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_PLUTO_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// UI 에서 BeginPlay() 역할을 해주는 함수
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* NowHPText;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* MaxHPText;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UProgressBar* HPBar;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UProgressBar* HPInnerBar;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UBossStateWidget* BossStateWidget;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UGameOverWidget* GameOverWidget;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UGameClearWidget* GameClearWidget;

	void SetNowHP( int32 NowHP );
	void SetMaxHP( int32 MaxHP );

	void SetHP( int32 Cur, int32 Max );

	void ShowBossState( bool bShow );
	void ShowGameOver( bool bShow );
	void ShowGameClear( bool bShow );

	void SetBossName( FString name );
};
