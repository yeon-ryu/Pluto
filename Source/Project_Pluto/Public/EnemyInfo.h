// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyInfo.generated.h"

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	Charge = 0 UMETA(DisplayName = "Charge"),
	CurtainFire UMETA(DisplayName = "CurtainFire"),
	Plate UMETA(DisplayName = "Plate"),
	Invalid

};

UCLASS(Abstract)
class PROJECT_PLUTO_API AEnemyInfo : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyInfo();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


protected:

	#pragma region DefaultStat
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "DefaultStat")
	float Damage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "DefaultStat")
	float NowHp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "DefaultStat")
	float MaxHp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "DefaultStat")
	float AttRange;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "DefaultStat")
	float DetectRange;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "DefaultStat")
	float Speed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "DefaultStat")
	EAttackType AttTypeEnum;

#pragma endregion DefaultStat


#pragma region Getter
public:
	FORCEINLINE int32 GetDamage() { return this->Damage; }

	FORCEINLINE int32 GetNowHp() { return this->NowHp; }

	FORCEINLINE int32 GetMaxHp() { return this->MaxHp; }

	FORCEINLINE float GetAttRange() { return this->AttRange; }

	FORCEINLINE float GetDetectRange() { return this->DetectRange; }

	FORCEINLINE float GetSpeed() { return this->Speed; }


#pragma endregion Getter

#pragma region Setter

protected:
	FORCEINLINE void SetDamage(int32 damage_value) { this->Damage = damage_value; }

	FORCEINLINE void SetNowHp(int32 nowhp_value) { this->NowHp = nowhp_value; }

	FORCEINLINE void SetMaxHp(int32 maxhp_value) { this->MaxHp = maxhp_value; }

	FORCEINLINE void SetAttRange(float attrange_value) { this->AttRange = attrange_value; }

	FORCEINLINE void SetDetectRange(float detect_value) { this->DetectRange = detect_value; }

	FORCEINLINE void SetSpeed(float speed_value) {	this->Speed = speed_value;	}

#pragma endregion Setter


public:
	virtual void Charge();
	virtual void CurtainFire();

	FVector GetTargetFromMe();

	virtual void AttackPlayer(EAttackType type);

	void ReceiveDamage(float damageValue);



};
