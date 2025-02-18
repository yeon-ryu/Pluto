// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlateActor.generated.h"

UCLASS()
class PROJECT_PLUTO_API APlateActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlateActor();

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

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
	class UNiagaraSystem* ExplosionFXSystem;

	class UNiagaraComponent* ExplosionFXComponent;

	void ActivateExplosion();
	void DeactivateComponent();

	FVector SpawnPoint;

	//타이머 핸들러
	FTimerHandle ExplosionTimerHandler;
	FTimerHandle DeactivateTimerHandler;

	float nowTime = 0.f;


};
