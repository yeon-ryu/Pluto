// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MonsterSpawner.generated.h"

UCLASS()
class PROJECT_PLUTO_API AMonsterSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMonsterSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
	class USceneComponent* SceneComp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	class UNiagaraSystem* SpawnFXSystem;

	class UNiagaraComponent* SpawnFXComponent;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Setting")
	TSubclassOf <class AMonster> MonsterFactory;

	

	FTimerHandle NiagaraTimer;
	FTimerHandle SpawnTimer;

	void NiagaraCaller();
	void ActivateNiagara();
	void SpawnMonster();
	
	int32 spawnCount = 0;


};
