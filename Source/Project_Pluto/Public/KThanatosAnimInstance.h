// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "KThanatosAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_PLUTO_API UKThanatosAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = TPSThanatosAnim)
	float Speed = 0.f;

	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = TPSThanatosAnim)
	float Direction = 0.f;

	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = TPSThanatosAnim)
	bool IsAttack;
};
