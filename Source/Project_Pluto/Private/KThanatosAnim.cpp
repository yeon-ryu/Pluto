// Fill out your copyright notice in the Description page of Project Settings.


#include "KThanatosAnim.h"
#include "KThanatos.h"
#include "KThanatosFSM.h"

void UKThanatosAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	/*Cast<ATPSPlayer>(TryGetPawnOwner());*/
	AKThanatos* npc = Cast<AKThanatos>(TryGetPawnOwner());
	if (!npc) return;

	FVector velocity = npc->GetVelocity();
	FVector forwardVector = npc->GetActorForwardVector();

	Speed = FVector::DotProduct(velocity, forwardVector);

	FVector rightVector = npc->GetActorRightVector();
	Direction = FVector::DotProduct(velocity, rightVector);


	IsAttack = npc->fsm->GetIsAttack();
}
