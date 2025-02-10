// Fill out your copyright notice in the Description page of Project Settings.


#include "BossFSM.h"
#include "DebugMacro.h"
#include "Boss.h"
#include "Components/CapsuleComponent.h"


// Sets default values for this component's properties
UBossFSM::UBossFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBossFSM::BeginPlay()
{
	Super::BeginPlay();

	//...

	me = Cast<ABoss>(GetOwner());
	
}


// Called every frame
void UBossFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	FString logMsg = UEnum::GetValueAsString(state);
	//GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Red, logMsg);

	Debug::Print(logMsg);
	

	switch (state)
	{
		case EBossState::Idle:		{ State_Idle(); }			break;
		case EBossState::Move:		{ State_Move(); }			break;
		case EBossState::Attack:		{ State_Attack(); }		break;
		case EBossState::Hit:			{ State_Hit(); }			break;
		case EBossState::PhaseChange:	{ State_PhaseChange(); }	break;
		case EBossState::Die:			{ State_Die(); }			break;
	}


}

void UBossFSM::State_Idle()
{
	//nowTime += GetDeltaTime();
	SetDesDir();

	if (direction.Size() <= me->GetDetectRange())
	{
		state = EBossState::Move;

		//nowTime = 0.0f;
	}
	
}

void UBossFSM::State_Move()
{
	SetDesDir();
	if (direction.Size() <= me->GetAttRange())
	{
		state = EBossState::Attack;
	}

	FRotator TargetRotation = direction.Rotation();

	FRotator NewRotation = me->GetActorRotation();
	NewRotation.Yaw = TargetRotation.Yaw;

	me->SetActorRotation(NewRotation);

	me->AddMovementInput(direction.GetSafeNormal());


}

void UBossFSM::State_Attack()
{
	int32 randnum = FMath::RandRange(0, me->GetAttPatternslength ()-1);
	me->SelectPattern(randnum);

	SetDesDir();
	if (direction.Size() >= me->GetAttRange())
	{
		state = EBossState::Idle;
	}

}

void UBossFSM::State_Hit()
{

	float percent = me->GetNowHp() / me->GetMaxHp();

	if (percent <= 0.75f && percent > 0.5f)
	{
		state = EBossState::PhaseChange;
	}

	if (percent <= 0.5f && percent > 0.25f)
	{
		state = EBossState::PhaseChange;
	}

	if (percent <= 0.25f && percent > 0.f)
	{
		state = EBossState::PhaseChange;
	}

	if (percent <= 0)
	{
		state = EBossState::Die;
	}

}

void UBossFSM::State_PhaseChange()
{
	me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	nowTime += GetDeltaTime();

	if (nowTime > PhaseChangeTime)
	{
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		nowTime = 0.f;
		state = EBossState::Move;
	}
}

void UBossFSM::State_Die()
{
	nowTime += GetDeltaTime();

	if (nowTime > 3.0f)
	{
		me->Destroy();
	}
}

void UBossFSM::SetDesDir()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	this->destination = PlayerController->GetPawn()->GetActorLocation();

	this->direction = this->destination - me->GetActorLocation();
}

void UBossFSM::OnTakeDamage()
{
	if (me->GetNowHp() > 0)
	{
		state = EBossState::Hit;
	}
	else
	{
		state = EBossState::Die;
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

