// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MonsterSpawner.h"
#include "SpawnManager.generated.h"

UCLASS()
class PROJECT_PLUTO_API ASpawnManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(EditAnywhere, Category = "Setting")
	TArray<AMonsterSpawner*> spawnPoints;

	void WhenMonsterDestroyed();
	void SpawnerCaller();
	void FindSpawnPoints();

	float nowTime = 0.f;
	bool bSpawnable = true;


private:
	int32 monsterCount = 0;
	int32 maxSpawnLoop = 3;
	int32 spawnLoop = 0;

};
