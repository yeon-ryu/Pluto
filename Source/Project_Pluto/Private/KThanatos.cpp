// Fill out your copyright notice in the Description page of Project Settings.


#include "KThanatos.h"
#include "KThanatosFSM.h"
#include "Engine/SkeletalMesh.h"

// Sets default values
AKThanatos::AKThanatos()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/KJY/RSS/ParagonSevarog/Characters/Heroes/Sevarog/Meshes/Sevarog.Sevarog'"));


	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -70.0f), FRotator(0.0f, -90.0f, 0.0f));
		GetMesh()->SetRelativeScale3D(FVector(1.0f));
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

