// Fill out your copyright notice in the Description page of Project Settings.


#include "KThanatosAnim.h"
#include "KThanatos.h"
#include "KThanatosFSM.h"

void UKThanatosAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	AKThanatos* thanatos = Cast<AKThanatos>(TryGetPawnOwner());

	if (!thanatos) return;

	FVector velocity = thanatos->GetVelocity();
	FVector forwardVector = thanatos->GetActorForwardVector();

	Speed = FVector::DotProduct(velocity, forwardVector);

	FVector rightVector = thanatos->GetActorRightVector();
	Direction = FVector::DotProduct(velocity, rightVector);

}

void UKThanatosAnim::AnimNotify_AttackEnd()
{
	if (AKThanatos* thanatos = Cast<AKThanatos>( TryGetPawnOwner() ))
	{
		thanatos->fsm->OnAttackEnd();
	}
}
