// Fill out your copyright notice in the Description page of Project Settings.


#include "BossAnimInstance.h"
#include "BossFSM.h"
#include "Boss.h"
#include "DebugMacro.h"

void UBossAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	ABoss* Boss = Cast<ABoss>(TryGetPawnOwner());

	if (Boss == nullptr)
	{
		return;
	}

	type = Boss->GetAttackType();

}

void UBossAnimInstance::AnimNotify_AttackEnd()
{

	bAttackPlay = false;
	bossfsm->state = EBossState::Attack_End;
}
