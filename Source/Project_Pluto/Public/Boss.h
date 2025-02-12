// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyInfo.h"
#include "Boss.generated.h"

UCLASS()
class PROJECT_PLUTO_API ABoss : public AEnemyInfo
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABoss();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


public:

#pragma region Getter/Setter
	FORCEINLINE int32 GetDamage( int32 index ) { return this->AttDamages[index]; }

	FORCEINLINE int32 GetNowHp() { return this->NowHp; }
	FORCEINLINE void SetNowHp(int32 nowhp_value) { this->NowHp = nowhp_value; }

	FORCEINLINE int32 GetMaxHp() { return this->MaxHp; }
	FORCEINLINE void SetMaxHp(int32 maxhp_value) { this->MaxHp = maxhp_value; }

	FORCEINLINE float GetAttRange() { return this->AttRange; }
	FORCEINLINE void SetAttRange(float attrange_value) { this->AttRange = attrange_value; }

	FORCEINLINE float GetCoolDown() { return this->AttCoolDown; }
	FORCEINLINE void SetCoolDown(float cooldown_value) { this->AttCoolDown = cooldown_value; }

	FORCEINLINE float GetDetectRange() { return this->DetectRange; }
	FORCEINLINE void SetDetectRange(float detect_value) { this->DetectRange = detect_value; }

	FORCEINLINE float GetSpeed() { return this->Speed; }
	FORCEINLINE void SetSpeed(float speed_value) { this->Speed = speed_value; }

	FORCEINLINE int32 GetAttPatternslength() { return AttPatterns.Num(); }
#pragma endregion Getter/Setter

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "FSMComponent")
	class UBossFSM* fsm;

	void SelectPattern(int32 idx);

	//virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

private:
	float HpPercent;

	TArray<EAttackType> AttPatterns;
	TArray<int32> AttDamages = { 13, 4, 8 };

};
