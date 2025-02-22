// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAnimInstance.h"
#include "Monster.h"


void UMonsterAnimInstance::AnimNotify_AttackEnd()
{
	bAttackPlay = false;
	MonsterFSM->mState = EMonsterState::Attack_End;
}

void UMonsterAnimInstance::AnimNotify_DieEnd()
{
	AMonster* monster = Cast<AMonster>(TryGetPawnOwner());
	if (monster	 != nullptr)
	{
		monster->FSM->OnDieEnd();
	}
}
