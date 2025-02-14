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
	float AttCoolDown;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "DefaultStat")
	float DetectRange;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "DefaultStat")
	float Speed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "DefaultStat")
	EAttackType AttTypeEnum;

	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "DefaultStat")
	


#pragma endregion DefaultStat

public:
	virtual void Charge();
	virtual void CurtainFire();
	virtual void Plate();

	FVector GetTargetFromMe();

	virtual void AttackPlayer(EAttackType type);

	void ReceiveDamage(float damageValue);
	

};
