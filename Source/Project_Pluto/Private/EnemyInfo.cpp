// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyInfo.h"

// Sets default values
AEnemyInfo::AEnemyInfo()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyInfo::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyInfo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyInfo::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyInfo::Charge()
{
	FVector dir = GetTargetFromMe();

}

void AEnemyInfo::CurtainFire()
{

}

void AEnemyInfo::Plate()
{

}

FVector AEnemyInfo::GetTargetFromMe()
{
	APlayerController* controller = GetWorld()->GetFirstPlayerController();
	FVector playerLocation = controller->GetPawn()->GetActorLocation();

	return playerLocation - this->GetActorLocation();

}

void AEnemyInfo::ReceiveDamage(float damageValue)
{
	NowHp = FMath::Clamp(NowHp - damageValue, 0.f, MaxHp);

}

