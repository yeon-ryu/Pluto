// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerWeapon.h"
#include "PWBlade.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_PLUTO_API APWBlade : public APlayerWeapon
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	APWBlade();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


public:
	virtual void Attack(AActor* OtherActor) override;
	virtual void SpecialAtt(AActor* OtherActor) override;

private:
	// overlap 이벤트에서 적인거 확인하고 넘기기
	void Strike(class AEnemyInfo* Enemy);
	void Chop(class AEnemyInfo* Enemy);
	void Thrust(class AEnemyInfo* Enemy);
	void NovaSmash(class AEnemyInfo* Enemy);
	void Knockback(class AEnemyInfo* Enemy);
	void BackstabBan(class AEnemyInfo* Enemy);

public:
	// 무기 모양
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* MeshComp;

	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    
    
	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceStart;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceEnd;
};
