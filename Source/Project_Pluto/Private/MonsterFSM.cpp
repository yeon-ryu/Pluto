// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterFSM.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerZagreus.h"
#include "Monster.h"
#include "Components/CapsuleComponent.h"
#include "MonsterAnimInstance.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "NavigationSystem.h"
#include "Runtime/AIModule/Classes/Navigation/PathFollowingComponent.h"
#include "DebugMacro.h"

// Sets default values for this component's properties
UMonsterFSM::UMonsterFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMonsterFSM::BeginPlay()
{
	Super::BeginPlay();

	// ...

	AActor* actor = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerZagreus::StaticClass());
	if (actor)
		target = Cast<APlayerZagreus>(actor);

	me = Cast<AMonster>(GetOwner());

	if (me)
	{
		Anim = Cast<UMonsterAnimInstance>(me->GetMesh ()->GetAnimInstance ()); 
		Anim->MonsterFSM = this;
	}
	
	AI = Cast <AAIController>(me->GetController ());

	
}


// Called every frame
void UMonsterFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	FString logMsg = UEnum::GetValueAsString(mState);
	Debug::Print(logMsg);

	switch (mState)
	{
	case EMonsterState::Idle: { State_Idle(); }	break;
	case EMonsterState::Move: { State_Move(); }	break;
	case EMonsterState::Attack_Start: { State_Attack_Start(); }	break;
	case EMonsterState::Attack_End: { State_Attack_End(); }	break;
	case EMonsterState::Damage: { State_Damage(); }	break;
	case EMonsterState::Die: { State_Die(); }	break;
	}

}

void UMonsterFSM::State_Idle()
{
	currentTime += GetWorld()->DeltaTimeSeconds;


	if (me->GetTargetFromMe ().Size() <= me->GetDetectRange() * 10.f)
	{
		//새로운 랜덤 위치 가져오기
		GetRandomPosInNavMesh(me->GetActorLocation(), me->GetAttRange(), randomPos);

		// 이동 상태로 전이하고 싶다.
		mState = EMonsterState::Move;

		Anim->AnimState = mState;

		
	}

}

void UMonsterFSM::State_Move()
{
	if (!target || !me)	return;

	FVector destination = target->GetActorLocation();

	FVector dir = destination - me->GetActorLocation();

	//me->AddMovementInput(dir);

	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	// 목적지 길찾기 경로 데이터 검색
	FPathFindingQuery query;
	FAIMoveRequest req;

	// 목적지에서 인지할 수 있는 범위
	req.SetAcceptanceRadius(3.f);
	req.SetGoalLocation(dir);

	// 길찾기 위한 쿼리 생성
	AI->BuildPathfindingQuery(req, query);

	// 길찾기 결과를 가져오기
	FPathFindingResult result = ns->FindPathSync(query);

	// 목적지까지 길 찾기 성공 여부 확인
	if (result.Result == ENavigationQueryResult::Success)
	{
		//타겟으로 이동
		AI->MoveToLocation(dir);
	}
	else
	{
		//랜덤 위치로 이동
		auto randResult =	AI->MoveToLocation(randomPos);

		//목적지에 도착하면
		if (randResult == EPathFollowingRequestResult::AlreadyAtGoal)
		{
			//새로운 랜덤 위치 가져오기
			GetRandomPosInNavMesh(me->GetActorLocation(), me->GetAttRange (), randomPos);
		}

	}
	
	
	//AI->MoveToLocation(dir);

	if (dir.Size() <= me->GetAttRange())
	{
		mState = EMonsterState::Attack_Start;

		Anim->AnimState = mState;

		currentTime = attackDelayTime - 0.5f;

	}
	
	

}

void UMonsterFSM::State_Attack_Start()
{
	if (Anim->bAttackPlay == true) return;
	
	AI->StopMovement ();
	currentTime += GetWorld()->DeltaTimeSeconds;

	if (currentTime > attackDelayTime)
	{
		me->CurtainFire ();
		Anim->bAttackPlay = true;

		currentTime = 0.f;
	}

	float dist = FVector::Dist(me->GetActorLocation(), target->GetActorLocation());

	if (dist > me->GetAttRange())
	{
		mState = EMonsterState::Move;

		Anim->AnimState = mState;
	}

}

void UMonsterFSM::State_Attack_End()
{

	currentTime += GetWorld ()->DeltaTimeSeconds;

	if (currentTime > attackDelayTime)
	{
		mState = EMonsterState::Idle;
		Anim->AnimState = mState;
		currentTime = 0.f;

		//새로운 랜덤 위치 가져오기
		GetRandomPosInNavMesh(me->GetActorLocation(), 500.f, randomPos);
	}

}


void UMonsterFSM::State_Damage()
{
	AI->StopMovement ();

	currentTime += GetWorld()->DeltaTimeSeconds;

	if (currentTime > damageDelayTime)
	{
		mState = EMonsterState::Idle;
		currentTime = 0.f;

		Anim->AnimState = mState;
	}
}

void UMonsterFSM::State_Die()
{
	if(!bDieDone) return;

	FVector p0 = me->GetActorLocation();
	FVector vt = FVector::DownVector * disapperSpeed * GetWorld()->DeltaTimeSeconds;
	FVector p = p0 + vt;
	me->SetActorLocation(p);

	if (p.Z < -100.f)
	{
		me->Destroy();
	}

}

void UMonsterFSM::OnDamageProcess()
{
	if (me->GetNowHp() > 0.f)
	{
		mState = EMonsterState::Damage;

	}
	else
	{
		mState = EMonsterState::Die;
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//Anim->Montage_Play (Anim->MonsterMontage);
		me->PlayAnimMontage (Anim->MonsterMontage);
		
	}

	Anim->AnimState = mState;
}

bool UMonsterFSM::GetRandomPosInNavMesh(FVector centerLocation, float radius, FVector& dest)
{
	auto ns = UNavigationSystemV1::GetNavigationSystem (GetWorld ());

	FNavLocation loc;

	bool result = ns->GetRandomReachablePointInRadius(centerLocation, radius, loc);

	dest = loc.Location;
	return result;
}

