// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CurtainFireProjectile.generated.h"

UCLASS()
class PROJECT_PLUTO_API ACurtainFireProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACurtainFireProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Projectile")
	class UStaticMeshComponent* MeshComp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Projectile")
	class USphereComponent* SphereComp;


	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Projectile")
	class UProjectileMovementComponent* Movement;


	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Projectile")
	float LifeTime = 5.f;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Projectile")
	float Speed = 1000.f;


	void SetVelocity(FVector dir_Value);

};
