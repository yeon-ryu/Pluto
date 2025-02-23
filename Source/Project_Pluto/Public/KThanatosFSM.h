// Fill out your copyright notice in the Description page of Project Settings.

//cpp 구현부 교체를 위한 define
//109줄 클래스 타입 변경 + 해당 define 변경시 Enemy class 변경 가능
//TArray 사용할 시 고려사항
// ㄴ 에너미 로드시 배열생성, 에너미 죽을때마다 정보값 삭제.
// 1. 타나토스(혹은 어느 다른 한 곳)에 데이터 할당, 플레이어 + NPC 모두 해당 정보를 체크하기
// 2. 게임모드에서 데이터 관리하기


#define ENEMYCLASS AMonster

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KThanatos.h"
#include "KThanatosFSM.generated.h"

UENUM(BlueprintType)
enum class EThanatosState : uint8
{
	Start	UMETA(DisplayName = "Start"),
	Idle	UMETA(DisplayName = "Idle"),
	Idle2	UMETA(DisplayName = "Idle2"),
	Move	UMETA(DisplayName = "Move"),
	MoveFar UMETA(DisplayName = "MoveFar"),
	Attack1	UMETA(DisplayName = "Attack_1"),
	Attack2	UMETA(DisplayName = "Attack_2"),
	Damaged	UMETA(DisplayName = "Damage"),
	Dead	UMETA(DisplayName = "Dead"),
	EndGame	UMETA(DisplayName = "EndGame"),
	EndState,
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_PLUTO_API UKThanatosFSM : public UActorComponent
{
	GENERATED_BODY()


public:
	UKThanatosFSM();

	/*
public:
	UPROPERTY()
	class AHadesGameMode* GM;
	*/

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	struct Status
	{

	//타나토스는 HP개념이 없다 (무적)
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	//int32 hp;

	//Idle (대기모션) 행하는 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float idleDelayTime = 0.5f;

	//Idle (대기모션) 행하는 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float idleDelayTime2 = 3.0f;


	//걷는 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float walkingTime = 1.0f;

	//공격 사거리
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float attackRange = 1000.0f;

	//공격 딜레이 타임
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float attackDelayTime_1 = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float attackDelayTime_2 = 2.0f;
	

	};


public:
	//State 관련 처리
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = FSM)
	EThanatosState mState = EThanatosState::Start;

public:

	//기본 State
	void State_Start();
	void State_Idle();
	void State_Idle2();
	void State_Move();
	void State_MoveFar();
	void State_Attack1();
	void State_Attack2();


	//데미지 관련 함수
	void OnDamagedProcess();
	//공격 관련 함수
	void OnAttackProcess();
	void EndAttackProcess();
	//bool GetIsAttack();

	//수업자료 OnAttackEnd
	void OnAttackEnd();


	void SearchEnemy();

	void State_EndGame();


public:


	
	bool bEndGame = false;
	bool bCanDestroySelf = false;

	//경과시간
	UPROPERTY(EditAnywhere, Category = FSM)
	float currentTime = 0.0f;
	float attackCurrent = 0.0f;

	UPROPERTY(VisibleAnywhere, Category = FSM)
	class AMonster* target_Enemy;
	class AMonster* before_Target;



	//TArray<AActor*> target_Enemy_Array;

	//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//	TSubclassOf <class AKDestroyBox> boxPos;

	class AKDestroyBox* destroyBox;

	class AKSkillBox* skillBox;

	int skillCount = 0;
	int maxSkillCount = 1;

	int endGameCount = 0;

	

	Status status;

	//소유 액터
	UPROPERTY()
	class AKThanatos* me;

	UPROPERTY()
	class UKThanatosAnim* Anim;

//	FVector firstPos = FVector(3695, -240, 170);
//	FVector seconfPos = FVector(3600, -240, 170);
	FVector lastPos = FVector(3600, -240, 170);
	FVector Location_Zero;


	int32 checkPoint = 0;
	FVector ReturnDest();
};
