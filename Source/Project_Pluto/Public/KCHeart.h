// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KCHeart.generated.h"

UCLASS()
class PROJECT_PLUTO_API AKCHeart : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKCHeart();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Component)
	class UBoxComponent* BoxComp;

	class APlayerZagreus* player;

	class AKThanatos* thanatos;

UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
