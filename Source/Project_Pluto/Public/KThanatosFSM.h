// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KEnemyFSM.h"
#include "KThanatos.h"
#include "KThanatosFSM.generated.h"



UCLASS()
class PROJECT_PLUTO_API UKThanatosFSM : public UKEnemyFSM
{
	GENERATED_BODY()


public:
	UKThanatosFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:

	//기본 State
	void State_Idle()override;
	void State_Move()override;
	void State_Attack()override;
	void State_Attack2();
	void State_MoveFar()override;

	//데미지 관련 함수
	void OnDamagedProcess()override;
	//공격 관련 함수
	void OnAttackProcess()override;
	bool GetIsAttack();


public:
	//대기 시간
	UPROPERTY(EditAnywhere, Category = FSM)
	float IdleDelayTime = 2.0f;

	//대기 시간
	UPROPERTY(EditAnywhere, Category = FSM)
	float WalkingTime = 1.0f;


	//경과시간
	UPROPERTY(EditAnywhere, Category = FSM)
	float currentTime = 0.0f;

	//공격 범위
	UPROPERTY(EditAnywhere, Category = FSM)
	float attackRange = 1000.0f;
	
	UPROPERTY(VisibleAnywhere, Category = FSM)
	class AKEnemy* target_Enemy;

	int count = 0;

	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = FSM)
	bool IsAttack;


	//소유 액터
	UPROPERTY()
	class AKThanatos* me;

	 Status status;


public:

};
