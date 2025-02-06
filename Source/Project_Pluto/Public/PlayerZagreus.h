// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputActionValue.h"
#include "PlayerZagreus.generated.h"

UCLASS()
class PROJECT_PLUTO_API APlayerZagreus : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerZagreus();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


public:
	UPROPERTY(EditAnywhere, Category = Camera)
	class USpringArmComponent* springArmComp;

	UPROPERTY(EditAnywhere, Category = Camera)
	class UCameraComponent* camComp;



	FVector AttackDirection;

	FVector MouseLocation;

	// 공격할 때 마우스 위치 및 방향
	void SetAttackDir();


	UPROPERTY(EditAnywhere, Category="PlayerSetting")
	float Speed = 700.0f;

	FVector PlayerDir;


public:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext* IMC_Player;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Move;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Attack;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Dodge;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_SpecialAtt;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Spell;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Interaction;


	void Move(const FInputActionValue& inputValue); // 이동
	void Attack(const FInputActionValue& inputValue); // 일반 공격 & 콤보
	void Dodge(const FInputActionValue& inputValue); // 회피 & 대시
	void SpecialAtt(const FInputActionValue& inputValue); // 특수 공격 (Q 스킬)
	void Spell(const FInputActionValue& inputValue); // 마법 (우클릭)
	void Interaction(const FInputActionValue& inputValue); // 상호작용 (E 버튼)
};
