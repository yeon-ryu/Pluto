// Fill out your copyright notice in the Description page of Project Settings.


#include "KEnemy.h"
#include "KThanatosFSM.h"

// Sets default values
AKEnemy::AKEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

	PrimaryActorTick.bCanEverTick = true;

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
	
	OnDeadProcess(DeltaTime);



}

// Called to bind functionality to input
void AKEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void AKEnemy::OnDeadProcess(float DeltaTime)
{
	if (bSoonDead == true)
	{	/*
		if (currentTime < deadDelayTime)
		{
			currentTime += DeltaTime;
		}

		else
		{
			//혹은 사망모션 + 일정 딜레이 후 파괴
			this->Destroy();
			currentTime = 0.f;
		}
		*/
		//혹은 사망모션 + 일정 딜레이 후 파괴
		this->Destroy();
		currentTime = 0.f;
	}

}

void AKEnemy::SetbSoonDead()
{
	bSoonDead = true;

	//에너미 머리 위에 오브젝트 소환하기
	
}

