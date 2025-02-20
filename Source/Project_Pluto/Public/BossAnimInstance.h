// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BossFSM.h"
#include "Boss.h"
#include "BossAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_PLUTO_API UBossAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "FSM")
	class UBossFSM* bossfsm;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "FSM")
	EBossState animState;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "FSM")
	bool bAttackPlay = false;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "FSM")
	EAttackType type;

	UPROPERTY(EditDefaultsOnly)
	ABoss* Boss;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Montage")
	class UAnimMontage* BossMontage;

	UFUNCTION()
	void AnimNotify_AttackEnd();


};
