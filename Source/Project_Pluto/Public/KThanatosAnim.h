// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "KThanatosFSM.h"
#include "KThanatosAnim.generated.h"


UCLASS()
class PROJECT_PLUTO_API UKThanatosAnim : public UAnimInstance
{
	GENERATED_BODY()
	
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = TPSThanatosAnim)
	float Speed = 0.f;

	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = TPSThanatosAnim)
	float Direction = 0.f;

	
	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = FSM)
	EThanatosState AnimState = EThanatosState::Idle;

	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = FSM)
	bool bAttackPlay = false;

public:
	void AnimNotify_AttackEnd();

};
