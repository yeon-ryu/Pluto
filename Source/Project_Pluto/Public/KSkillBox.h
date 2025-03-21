// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KSkillBox.generated.h"

UCLASS()
class PROJECT_PLUTO_API AKSkillBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKSkillBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component)
	class UBoxComponent* BoxComp;



public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float curretTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DestroyDelay = 5.0f;

	bool bOnSpawn= false;

	void SetbOnSpawn();

};
