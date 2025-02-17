// Fill out your copyright notice in the Description page of Project Settings.


#include "KThanatosFSM.h"
#include "Kismet/GameplayStatics.h"
#include "KEnemy.h"
#include "KThanatosAnim.h"
#include "KDestroyBox.h"


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

	//스킬 1번 체크가 안 되어 있을때도 조건으로.
	if (actor_Enemy != nullptr)
	{
		target_Enemy = Cast<AKEnemy>(actor_Enemy);
	}


	//소유 객체 가져오기
	me = Cast<AKThanatos>(GetOwner());


	if (me)
	{
		Anim = Cast<UKThanatosAnim>(me->GetMesh()->GetAnimInstance());
		Anim->AnimState = mState;
	}

}

void UKThanatosFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 실행창에 상태 메시지 출력하기
	FString logMsg = UEnum::GetValueAsString(mState);
	GEngine->AddOnScreenDebugMessage(0, 1, FColor::Red, logMsg);
	

	switch (mState)
	{	
	case EThanatosState::Start: { State_Start(); } break;
	case EThanatosState::Idle: { State_Idle(); } break;
	case EThanatosState::Move: { State_Move(); } break;
	case EThanatosState::MoveFar: { State_MoveFar(); } break;
	case EThanatosState::Attack1: { State_Attack1(); } break;
	case EThanatosState::Attack2: { State_Attack2(); } break;
	}

}


void UKThanatosFSM::State_Start()
{
	currentTime += GetWorld()->DeltaTimeSeconds;

	if (currentTime > 3.0f)
	{
		//이동 상태로 전환
		mState = EThanatosState::Idle;
		Anim->AnimState = mState;

		//전환 후 시간 초기화
		currentTime = 0.0f;
	}

}


void UKThanatosFSM::State_Idle()
{
	//시간이 흐르다가
	currentTime += GetWorld()->DeltaTimeSeconds;

	//만약 경과 시간이 대기 시간을 초과했다면
	if (currentTime > status.idleDelayTime)
	{
		//이동 상태로 전환
		mState = EThanatosState::Move;
		Anim->AnimState = mState;

		//전환 후 시간 초기화
		currentTime = 0.0f;
	}

}

void UKThanatosFSM::State_Move()
{
	FVector destination = target_Enemy->GetActorLocation();
	FVector dir = destination - me->GetActorLocation();

	//1초(WalkingTime) 걸은 후엔 2초(IdleDelayTime) 대기하고 싶음
	currentTime += GetWorld()->DeltaTimeSeconds;

	if (dir.Size() > status.attackRange)
	{
		me->AddMovementInput(dir.GetSafeNormal());

		if (currentTime > status.walkingTime)
		{
			mState = EThanatosState::Idle;
			Anim->AnimState = mState;
			currentTime = 0.0f;
			//거리체크
		}
	}

	//(dir.Size() < status.attackRange)
	else 
	{
		//공격 상태로 전환하고 싶다
		OnAttackProcess();

		currentTime = 0.0f;
	}
}
 

void UKThanatosFSM::State_MoveFar()
{
	FVector destination = target_Enemy->GetActorLocation();
	FVector dir = me->GetActorLocation() - destination;

	me->AddMovementInput(dir.GetSafeNormal());


	//1초(WalkingTime) 걸은 후엔 2초(IdleDelayTime) 대기하고 싶음
	currentTime += GetWorld()->DeltaTimeSeconds;

	if (currentTime > status.walkingTime)
	{
		mState = EThanatosState::Idle;
		Anim->AnimState = mState;
		currentTime = 0.0f;
	}
}



void UKThanatosFSM::State_Attack1()
{
	//일정 시간에 한 번씩 공격

	//시간이 흐르다가
	currentTime += GetWorld()->DeltaTimeSeconds;
	//공격 시간이되면 공격을 실행
	if (currentTime > status.attackDelayTime)
	{

		++skillCount;
		if (skillCount == maxSkillCount)
		{
			status.attackRange = 2000.0f;
		}
		
		currentTime = 0.0f;


		EndAttackProcess();
	}
}


void UKThanatosFSM::State_Attack2()
{
	currentTime += GetWorld()->DeltaTimeSeconds;
	

	//공격 시간이되면 공격을 실행
	if (currentTime > status.attackDelayTime)
	{
		GetWorld()->SpawnActor<AKDestroyBox>(boxPos, target_Enemy->GetTransform());
		//GetWorld()->SpawnActor<AKDestroyBox>(boxPos, FTransform(FRotator(0, 0, 0), FVector(2000, 2000, 2000), FVector(1.0f, 1.0f, 1.0f)));
		skillCount =0;
		status.attackRange = 1000.0f;

		currentTime = 0.0f;

		EndAttackProcess();
	}
}

void UKThanatosFSM::OnDamagedProcess()
{
	//콜리전 박스 or 설정해둔 박스가 빛나게 하기.
}

void UKThanatosFSM::OnAttackProcess()
{
		//IsAttack = true;
		Anim->bAttackPlay = true;

		if (skillCount >= maxSkillCount)
		{
			mState = EThanatosState::Attack2;
			Anim->AnimState = mState;
		}

		else
		{
			mState = EThanatosState::Attack1;
			Anim->AnimState = mState;

		}
}


void UKThanatosFSM::EndAttackProcess()
{
	//테스트용 패트롤.
	mState = EThanatosState::Idle;
	Anim->AnimState = mState;

	OnAttackEnd();

	/*
	if (스킬 걸리지 않은 에너미가 있을 시)
	{
		그 쪽으로 향함.
	}

	else (랜덤 패트롤)
	*/
}



void UKThanatosFSM::OnAttackEnd()
{
	Anim->bAttackPlay = false;
}


