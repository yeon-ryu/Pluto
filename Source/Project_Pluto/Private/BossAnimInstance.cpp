// Fill out your copyright notice in the Description page of Project Settings.


#include "BossAnimInstance.h"
#include "Boss.h"

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
