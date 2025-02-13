// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerWeapon.generated.h"

UCLASS()
class PROJECT_PLUTO_API APlayerWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


protected:
	// 영구 은혜 등 읽어와서 적용해야할게 많기에? 불러온다
	UPROPERTY()
	class APlayerZagreus* player;

public:
	// 무기 overlap
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* CollisionComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combo")
	int32 MaxCombo = 1; // 각 무기별 최대 Attack 콤보


	// 순수 가상함수로 만들려니 헷갈려서 그냥 가상함수로
	virtual void Attack(AActor* OtherActor); // 일반 공격 & 콤보
	virtual void SpecialAtt(AActor* OtherActor); // 특수 공격 (Q 스킬)


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buff")
	float attackBuff = 0.0f; // 일반 공격 강화 (일시적 강화)

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
