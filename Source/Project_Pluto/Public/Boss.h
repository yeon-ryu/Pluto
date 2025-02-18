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
	FORCEINLINE int32 GetDamage() { return this->Damage; }
	FORCEINLINE void SetDamage(int32 damage_value) { this->Damage = damage_value; }

	FORCEINLINE int32 GetNowHp() { return this->NowHp; }
	FORCEINLINE void SetNowHp(int32 nowhp_value) { this->NowHp = nowhp_value; }

	FORCEINLINE int32 GetMaxHp() { return this->MaxHp; }
	FORCEINLINE void SetMaxHp(int32 maxhp_value) { this->MaxHp = maxhp_value; }

	FORCEINLINE float GetAttRange() { return this->AttRange; }
	FORCEINLINE void SetAttRange(float attrange_value) { this->AttRange= attrange_value; }

	FORCEINLINE float GetCoolDown() { return this->AttCoolDown; }
	FORCEINLINE void SetCoolDown(float cooldown_value) { this->AttCoolDown = cooldown_value; }

	FORCEINLINE float GetDetectRange() { return this->DetectRange; }
	FORCEINLINE void SetDetectRange(float detect_value) { this->DetectRange = detect_value; }

	FORCEINLINE float GetSpeed() { return this->Speed; }
	FORCEINLINE void SetSpeed(float speed_value) { this->Speed = speed_value; }

	FORCEINLINE int32 GetAttPatternslength() { return AttPatterns.Num(); }
	FORCEINLINE EAttackType GetAttackType() { return this->AttTypeEnum; }
#pragma endregion Getter/Setter

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "FSMComponent")
	class UBossFSM* fsm;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<class ACurtainFireProjectile> ProjectileFactory;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Projectile")
	class UArrowComponent* ArrowComp;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Niagara")
	TSubclassOf <class APlateActor> PlateFactory;


	UFUNCTION(BlueprintCallable, Category = "FSM")
	void SelectPattern(int32 idx);

	void AddAttPatterns();

	void AttackPlayer(EAttackType type) override;
	
	void Charge() override;
	void CurtainFire() override;
	void SpawnProjectile();
	void Plate();
	void SpawnPlate();

	FVector GetRandomPos(FVector pos);

	//타이머 핸들러
	FTimerHandle TimerHandle;

	//virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	//디버그용 함수
	UFUNCTION(BlueprintCallable, Category = "Debug")
	void SelectCharge();
	UFUNCTION(BlueprintCallable, Category = "Debug")
	void SelectCurtainFire();

public:
#pragma region Debug_Values

	UPROPERTY(EditAnywhere, Category = "DebugValue")
	float dxdyRange = 450.f;

	UPROPERTY(EditAnywhere, Category = "DebugValue")
	float minRange = 100.f;

	UPROPERTY(EditAnywhere, Category = "DebugValue")
	float maxRange = 500.f;


#pragma endregion Debug_Values

private:
	float HpPercent;

	TArray<EAttackType> AttPatterns;
	TArray<int32> AttDamages = { 13, 4, 8 };
	TArray<float> AttRanges = { 800.f, 10000.f, 10000.f };

	int32 fireCounter = 0;
	int32 plateCounter = 0;

	TArray<TArray<int32>> index = { {0,1,2},{0,1,3},{0,2,3},{1,2,3} };

};
