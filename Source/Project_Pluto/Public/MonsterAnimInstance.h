// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MonsterFSM.h"
#include "MonsterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_PLUTO_API UMonsterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "FSM")
	EMonsterState AnimState = EMonsterState::Idle;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "FSM")
	bool bAttackPlay = false;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "FSM")
	class UMonsterFSM* MonsterFSM;

	UFUNCTION()
	void AnimNotify_AttackEnd();

	UFUNCTION()
	void AnimNotify_DieEnd();

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Montage")
	class UAnimMontage* MonsterMontage;
	
};
