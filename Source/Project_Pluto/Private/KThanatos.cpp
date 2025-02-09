// Fill out your copyright notice in the Description page of Project Settings.


#include "KThanatos.h"
#include "KThanatosFSM.h"

// Sets default values
AKThanatos::AKThanatos()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));

	if (TempMesh.Succeeded())
	{
		//GetMesh()->SetMesh(TempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FRotator(0.0f, -90.0f, 0.0f));

	}

	fsm = CreateDefaultSubobject<UKThanatosFSM>(TEXT("FSM"));
	
}

// Called when the game starts or when spawned
void AKThanatos::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AKThanatos::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AKThanatos::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

