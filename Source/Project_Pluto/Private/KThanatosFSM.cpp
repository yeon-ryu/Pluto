// Fill out your copyright notice in the Description page of Project Settings.


#include "KThanatosFSM.h"
#include "Kismet/GameplayStatics.h"
#include "KEnemy.h"

UKThanatosFSM::UKThanatosFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}

void UKThanatosFSM::BeginPlay()
{
	Super::BeginPlay();

	status.attackDelayTime = 2.0f;
	// ...
	//월드에서 Enemy를 찾아오기
	auto actor_Enemy = UGameplayStatics::GetActorOfClass(GetWorld(), AKEnemy::StaticClass());

	if (actor_Enemy != nullptr)
	{
		target_Enemy = Cast<AKEnemy>(actor_Enemy);
	}

	//소유 객체 가져오기
	me = Cast<AKThanatos>(GetOwner());



}

void UKThanatosFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 실행창에 상태 메시지 출력하기
	FString logMsg = UEnum::GetValueAsString(mState);
	GEngine->AddOnScreenDebugMessage(0, 1, FColor::Red, logMsg);
	

	switch (mState)
	{
	case EKEnemyState::Idle:	{ State_Idle();		} break;
	case EKEnemyState::Move:	{ State_Move();		} break;
	case EKEnemyState::MoveFar: { State_MoveFar();	} break;
	case EKEnemyState::Attack:	{ State_Attack();	} break;
	case EKEnemyState::Attack2:	{ State_Attack2();	} break;
	}

}

void UKThanatosFSM::State_Idle()
{
	//시간이 흐르다가
	currentTime += GetWorld()->DeltaTimeSeconds;

	//만약 경과 시간이 대기 시간을 초과했다면
	if (currentTime > IdleDelayTime)
	{
		//이동 상태로 전환
		mState = EKEnemyState::Move;

		//전환 후 시간 초기화
		currentTime = 0.0f;
	}

}

void UKThanatosFSM::State_Move()
{
	FVector destination = target_Enemy->GetActorLocation();
	FVector dir = destination - me->GetActorLocation();
	me->AddMovementInput(dir.GetSafeNormal());

	//1초(WalkingTime) 걸은 후엔 2초(IdleDelayTime) 대기하고 싶음
	currentTime += GetWorld()->DeltaTimeSeconds;

	if (currentTime > WalkingTime)
	{
		mState = EKEnemyState::Idle;
		currentTime = 0.0f;
		//거리체크
		if (dir.Size() < attackRange)
		{
			//공격 상태로 전환하고 싶다
			mState = EKEnemyState::Attack;
			currentTime = 0.0f;
		}
	}

	


}

void UKThanatosFSM::State_Attack()
{
	//일정 시간에 한 번씩 공격

	//시간이 흐르다가
	currentTime += GetWorld()->DeltaTimeSeconds;
	//공격 시간이되면 공격을 실행
	if (currentTime > status.attackDelayTime)
	{
		currentTime = 0.0f;

		count++;
		mState = EKEnemyState::MoveFar;
		IsAttack = false;
	}
}


void UKThanatosFSM::State_Attack2()
{
	attackRange = 2000.0f;
	currentTime += GetWorld()->DeltaTimeSeconds;

	//공격 시간이되면 공격을 실행
	if (currentTime > status.attackDelayTime)
	{
		currentTime = 0.0f;
		
		count=0;
		mState = EKEnemyState::MoveFar;
	}

	attackRange = 1000.0f;
	IsAttack = false;
}

void UKThanatosFSM::State_MoveFar()
{
	FVector destination = target_Enemy->GetActorLocation();
	FVector dir = me->GetActorLocation() - destination;



	me->AddMovementInput(dir.GetSafeNormal());

	//1초(WalkingTime) 걸은 후엔 2초(IdleDelayTime) 대기하고 싶음
	currentTime += GetWorld()->DeltaTimeSeconds;

	if (currentTime > WalkingTime)
	{
		mState = EKEnemyState::Idle;
		currentTime = 0.0f;
	}

	
}

void UKThanatosFSM::OnDamagedProcess()
{
	//콜리전 박스 or 설정해둔 박스가 빛나게 하기.
}

void UKThanatosFSM::OnAttackProcess()
{
		IsAttack = true;


		if (count >= 10)
		{
			mState = EKEnemyState::Attack2;
		}

		else
		{
			mState = EKEnemyState::Attack;

		}
}

bool UKThanatosFSM::GetIsAttack()
{
	return IsAttack;
}

