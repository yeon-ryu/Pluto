// Fill out your copyright notice in the Description page of Project Settings.


#include "BossFSM.h"
#include "DebugMacro.h"
#include "Boss.h"
#include "BossAnimInstance.h"
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

	anim = Cast <UBossAnimInstance>(me->GetMesh()->GetAnimInstance());
	anim->bossfsm = this;
}


// Called every frame
void UBossFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	FString logMsg = UEnum::GetValueAsString(state);
	Debug::Print(logMsg);
	

	switch (state)
	{
		case EBossState::Idle:			{ State_Idle(); }				break;
		case EBossState::Move:			{ State_Move(); }				break;
		case EBossState::Attack_Start:		{ State_Attack_Start(); }		break;
		case EBossState::Attack_End:		{ State_Attack_End(); }		break;
		case EBossState::Hit:				{ State_Hit(); }				break;
		case EBossState::PhaseChange:		{ State_PhaseChange(); }		break;
		case EBossState::Die:				{ State_Die(); }				break;
	}


}

void UBossFSM::State_Idle()
{
	SetDesDir();

	if (direction.Size() <= me->GetDetectRange())
	{
		state = EBossState::Move;

		anim->animState = state;
	}
	
}

void UBossFSM::State_Move()
{
	if (state == EBossState::Attack_Start)
	{
		return;

	}
	//me->PlayAnimMontage(anim->BossMontage, 1.f, TEXT("Death"));
	

	CallSelectPattern();
	SetDesDir();
	if (direction.Size() <= BossAttRange)
	{
		state = EBossState::Attack_Start;
		anim->animState = state;
		nowTime = AttackDelayTime - 0.5f;
	}

	FRotator TargetRotation = direction.Rotation();

	FRotator NewRotation = me->GetActorRotation();
	NewRotation.Yaw = TargetRotation.Yaw;

	me->SetActorRotation(NewRotation);

	me->AddMovementInput(direction.GetSafeNormal());


}

void UBossFSM::State_Attack_Start()
{
	if (anim->bAttackPlay == true) return;
	SetDesDir();
	nowTime += GetDeltaTime();

	if (nowTime > AttackDelayTime)
	{
		me->AttackPlayer(me->GetAttackType());
		anim->bAttackPlay = true;
		ResetNowTime();
	}

}

void UBossFSM::State_Attack_End()
{

	nowTime += GetDeltaTime();

	if (nowTime > AttackDelayTime)
	{
		state = EBossState::Idle;
		anim->animState = state;
		ResetNowTime();
	}

}

void UBossFSM::State_Hit()
{
	SetForPhaseChange();
	float percent = me->GetNowHp() / me->GetMaxHp();

	if (!bPhaseChange && percent <= 0.75f && percent > 0.5f)
	{

		state = EBossState::PhaseChange;
		anim->animState = state;
		me->AddAttPatterns();
		bPhaseChange = true;

		

		return;
	}

	else if ( bPhaseChange && percent <= 0.5f && percent > 0.25f)
	{
		state = EBossState::PhaseChange;
		anim->animState = state;
		bPhaseChange = false;
		return;
	}

	else if ( !bPhaseChange && percent <= 0.25f && percent > 0.00f)
	{
		state = EBossState::PhaseChange;
		anim->animState = state;
		me->AddAttPatterns();
		bPhaseChange = true;
		return;
	}

	else if (percent == 0.00f)
	{
		
		state = EBossState::Die;
		return;
	}

	else
	{
		state = EBossState::Idle;
		anim->animState = state;
		return;
	}
	

}

void UBossFSM::State_PhaseChange()
{
	me->GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECR_Ignore);


	nowTime += GetDeltaTime();

	if (nowTime > PhaseChangeTime)
	{
		me->GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECR_Block);
		ResetNowTime();
		state = EBossState::Idle;
		anim->animState = state;
	}
}

void UBossFSM::State_Die()
{
	if (!bDieDone)
	{
		return;
	}

	me->Destroy();

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
		anim->Montage_Play(anim->BossMontage);
		state = EBossState::Die;
		me->GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECR_Ignore);
	}
	//anim->animState = state;
}

void UBossFSM::CallSelectPattern()
{
	int32 randnum = FMath::RandRange(0, me->GetAttPatternslength() - 1);
	me->SelectPattern(randnum);

	BossAttDamage = me->GetDamage();
	BossAttRange = me->GetAttRange();

}

void UBossFSM::SetForPhaseChange()
{
	if (anim->bAttackPlay)
	{
		anim->bAttackPlay = false;

	}
	else
		return;
}

