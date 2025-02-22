// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "KEnemy.generated.h"

UCLASS()
class PROJECT_PLUTO_API AKEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AKEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	class UKEnemyFSM* fsm;

public:
	//타나토스 1번 스킬이 걸릴 경우 true 체크
	bool bSoonDead = false;

	
	float currentTime = 0.f;
	float deadDelayTime = 2.f;

	
	//bSoonDead가 True일 경우 deadDelayTime 체크해서 디스트로이
	void OnDeadProcess(float DeltaTime);

	//bSoonDead를 true로 설정해주는 함수
	void SetbSoonDead();
	
};