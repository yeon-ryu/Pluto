// Fill out your copyright notice in the Description page of Project Settings.

//박스를 해당 로케이션에 소환 - 삭제하려고 했더니 좌표가 겹치면 소환이 안된다!!
//2025.02.18 박스를 미리 맵 밖에 올려두고 필요할때 끌어치기로 변경시도
// ㄴ FSM 190줄에 주석처리
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KDestroyBox.generated.h"

UCLASS()
class PROJECT_PLUTO_API AKDestroyBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKDestroyBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component)
	class UBoxComponent* BoxComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component)
	class UStaticMeshComponent* MeshComp;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float curretTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DestroyDelay = 3.0f;

	bool bOnSpawn= false;

	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void SetbOnSpawn();


};
