// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "BossFSM.h"



// Sets default values
ABoss::ABoss()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/MW/Character/ParagonMorigesh/Characters/Heroes/Morigesh/Skins/DarkHeart/Meshes/Morigesh_DarkHeart.Morigesh_DarkHeart'"));

	// 만약 파일 읽기가 성공했다면,
	if (TempMesh.Succeeded())
	{
		// 로드한 메쉬를 넣어주고 싶다.
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		// 위치값과 회전값을 반영하고 싶다.
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FRotator(0.0f, -90.0f, 0.0f));
	}

	// Boss Component 추가
	fsm = CreateDefaultSubobject<UBossFSM>(TEXT("BossFSM"));

#pragma region initStatSetting
	SetMaxHp(4400);

	SetDetectRange(10000.0f);

	SetAttRange(1000.0f);

	//기본 공격패턴인 Charge 추가
	AttPatterns.Add(EAttackType::Charge);

#pragma endregion initSetting


	

}

// Called when the game starts or when spawned
void ABoss::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABoss::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABoss::SelectPattern(int32 idx)
{
	switch (idx)
	{
		case 0:
			{
				AttTypeEnum = EAttackType::Charge;
				break;
			}
		case 1:
			{
				AttTypeEnum = EAttackType::CurtainFire;
				break;
			}
		case 2:
			{
				AttTypeEnum = EAttackType::Plate;
				break;
			}

	}
}

