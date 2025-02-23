// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "HadesSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_PLUTO_API UHadesSaveGame : public ULocalPlayerSaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite)
	int32 HPBuffSave;
};
