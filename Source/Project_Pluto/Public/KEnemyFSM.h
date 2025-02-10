// Fill out your copyright notice in the Description page of Project Settings.

//플레이어 클래스 명, 에너미(타나토스) 클래스명이 없다!


#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KEnemyFSM.generated.h"


UENUM(BlueprintType)
enum class EKEnemyState : uint8
{
	Idle	UMETA(DisplayName = "대기"),
	Move	UMETA(DisplayName = "이동"),
	MoveFar UMETA(DisplayName = "이동2"),
	Attack	UMETA(DisplayName = "공격"),
	Damaged	UMETA(DisplayName = "데미지"),
	Dead	UMETA(DisplayName = "사망"),
	EndState,
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_PLUTO_API UKEnemyFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UKEnemyFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	//State 관련 처리
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = FSM)
	EKEnemyState mState = EKEnemyState::Idle;

	//기본 State
	virtual void State_Idle();
	virtual void State_Move();
	virtual void State_MoveFar();
	virtual void State_Attack();
	virtual void State_Damaged();
	virtual void State_Dead();

	//데미지 관련 함수
	virtual void OnDamagedProcess();
	//공격 관련 함수
	virtual void OnAttackProcess();

public:

	struct Status
	{

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	int32 hp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float attackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float attackDelayTime;
	};

	
	//상속받아야 할 수치가 정해진 변수만 물려받는게 낫나?


	//타겟 (플레이어 클래스가 없어서 주석처리)
	//UPROPERTY(VisibleAnywhere, Category = FSM)
	//class ATPSPlayer* targetPlayer;

	//UPROPERTY(EditAnywhere, Category = FSM)

};
