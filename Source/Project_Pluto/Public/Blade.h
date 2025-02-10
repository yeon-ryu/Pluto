// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Blade.generated.h"

UCLASS()
class PROJECT_PLUTO_API ABlade : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABlade();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponSetting")
	int32 InitAtk = 20; // 초기 기본 공격력

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponSetting")
	int32 Atk = 20; // 기본 공격력


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buff")
	float atkBuff = 0.0f; // 일시적 강화


	void Strike();
	void Chop();
	void Thrust();
	void NovaSmash();
	void Knockback();
	void BackstabBan();

	UPROPERTY()
	class APlayerZagreus* player;
};
