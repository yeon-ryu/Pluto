// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "BossFSM.h"
#include "CurtainFireProjectile.h"
#include "Components/ArrowComponent.h"
#include "DebugMacro.h"
#include "Kismet/KismetMathLibrary.h"



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

	ConstructorHelpers::FClassFinder<UAnimInstance> tempClass(TEXT("/Script/Engine.AnimBlueprint'/Game/MW/Blueprints/ABP_Boss.ABP_Boss_C'"));
	if (tempClass.Succeeded ())
	{
		GetMesh()->SetAnimInstanceClass(tempClass.Class);
	}

	// Boss Component 추가
	fsm = CreateDefaultSubobject<UBossFSM>(TEXT("BossFSM"));

	ArrowComp = CreateDefaultSubobject <UArrowComponent>(TEXT("ArrowComp"));
	ArrowComp->SetupAttachment(RootComponent);
	ArrowComp->SetRelativeLocation(FVector(40.f, 0.f, 30.f));

#pragma region initStatSetting
	SetMaxHp(4400);

	SetDetectRange(10000.0f); //원래는 10000.f - 디버깅용 

	SetAttRange(1000.0f);

	SetDamage(0);

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
				SetDamage(AttDamages[idx]);
				SetAttRange(AttRanges[idx]);
				break;
			}
		case 1:
			{
				AttTypeEnum = EAttackType::CurtainFire;
				SetDamage(AttDamages[idx]);
				SetAttRange(AttRanges[idx]);
				break;
			}
		case 2:
			{
				AttTypeEnum = EAttackType::Plate;
				SetDamage(AttDamages[idx]);
				SetAttRange(AttRanges[idx]);
				break;
			}

	}
}

void ABoss::AddAttPatterns()
{
	float percent = (float)GetNowHp() / GetMaxHp();

	if (percent <= 0.75f && percent > 0.5f)
	{
		AttPatterns.Add(EAttackType::CurtainFire);
	}


	if (percent <= 0.25f && percent > 0.f)
	{
		AttPatterns.Add(EAttackType::Plate);
	}
}

void ABoss::AttackPlayer(EAttackType type)
{
	switch (type)
	{
	case EAttackType::Charge:			{ Charge();			break; }
	case EAttackType::CurtainFire:		{ CurtainFire();		break; }
	case EAttackType::Plate:			{ Plate();			break; }
	}
}

void ABoss::Charge()
{
	FVector dir = GetTargetFromMe();
	dir.Normalize();
	this->LaunchCharacter(dir * 12000.f , true, false);
}

void ABoss::CurtainFire()
{

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ABoss::SpawnProjectile, 0.4f, true, 4.f);

}

void ABoss::SpawnProjectile()
{
	if (fireCounter >= 10)
	{
		//10번 쏘면 타이머 끄기
		GetWorldTimerManager().ClearTimer(TimerHandle);
		fireCounter = 0;
		return;
	}

	FActorSpawnParameters spawnParams;
	spawnParams.bNoFail = true;
	spawnParams.Owner = this;

	//기준 각은 플레이어를 바라보게 하고싶다.
	APlayerController* controller = GetWorld()->GetFirstPlayerController();
	FVector playerLocation = controller->GetPawn()->GetActorLocation();
	FRotator BaseRotation = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation (), playerLocation);

	// 중앙 기준 각도
	float BaseAngle = BaseRotation.Yaw; 
	// 좌우로 퍼지는 각도 차이
	float AngleOffset = 15.0f; 

	for (int32 i = -2; i < 3; i++)
	{
		float angle = BaseAngle + (i * AngleOffset);
		FRotator fireAngle = FRotator(0.f, angle, 0.f);

		ACurtainFireProjectile* projectile = GetWorld()->SpawnActor <ACurtainFireProjectile>(ProjectileFactory, ArrowComp->GetComponentTransform(), spawnParams);

		
		projectile->SetVelocity(fireAngle.Vector ());
	}
	fireCounter++;

}

void ABoss::SelectCharge()
{
	AttTypeEnum = EAttackType::Charge;
	SetDamage(AttDamages[0]);
	SetAttRange(AttRanges[0]);

	fsm->BossAttDamage = GetDamage();
	fsm->BossAttRange = GetAttRange();
}

void ABoss::SelectCurtainFire()
{
	AttTypeEnum = EAttackType::CurtainFire;
	SetDamage(AttDamages[1]);
	SetAttRange(AttRanges[1]);

	fsm->BossAttDamage = GetDamage();
	fsm->BossAttRange = GetAttRange();
}

