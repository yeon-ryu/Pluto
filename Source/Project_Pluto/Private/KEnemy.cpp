// Fill out your copyright notice in the Description page of Project Settings.


#include "KEnemy.h"
#include "KThanatosFSM.h"

// Sets default values
AKEnemy::AKEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

	PrimaryActorTick.bCanEverTick = true;
	/*
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/ Script / Engine.SkeletalMesh'/Game/Characters/Mannequin_UE4/Meshes/SK_Mannequin.SK_Mannequin'"));

	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FRotator(0.0f, -90.0f, 0.0f));

	}

	fsm = CreateDefaultSubobject<UKThanatosFSM>(TEXT("FSM"));
	*/

}

// Called when the game starts or when spawned
void AKEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AKEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AKEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

