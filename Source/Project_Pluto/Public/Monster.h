// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyInfo.h"
#include "Monster.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_PLUTO_API AMonster : public AEnemyInfo
{
	GENERATED_BODY()
	
public:
	AMonster();

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "FSMComponent")
	class UMonsterFSM* FSM;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Thanatos")
	bool bSoonDead = false;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<class ACurtainFireProjectile> ProjectileFactory;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Projectile")
	class UArrowComponent* ArrowComp;

	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	FTimerHandle TimerHandle;
	FTimerHandle SingleTimer;

	void CurtainFire () override;
	void SpawnProjectile();
	void ShootSingle( );

	void Destroyed() override;

	FRotator BaseRotation;
	int32 fireCounter = 1;

};
