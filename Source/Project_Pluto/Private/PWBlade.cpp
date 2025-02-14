// Fill out your copyright notice in the Description page of Project Settings.


#include "PWBlade.h"
#include "EnemyInfo.h"
#include "PlayerZagreus.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"

APWBlade::APWBlade()
{
	CollisionComp->SetBoxExtent(FVector(10.0f, 10.0f, 60.0f));
	// CollisionComp->SetRelativeLocationAndRotation(FVector(60.0f, 5.0f, 80.0f), FRotator(-32.0f, -42.0f, 24.0f));
	CollisionComp->SetRelativeLocation(FVector(-6.0f, 4.0f, 105.0f));

	// 검 모양 액터
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BladeMeshComp"));
	MeshComp->SetupAttachment(CollisionComp);

	ConstructorHelpers::FObjectFinder<UStaticMesh>TempMesh(TEXT("/Script/Engine.StaticMesh'/Game/RGY/Modelings/Blade/stygian_blade_-_hades.stygian_blade_-_hades'"));

	if (TempMesh.Succeeded()) {
		MeshComp->SetStaticMesh(TempMesh.Object);
		MeshComp->SetRelativeScale3D(FVector(0.08f));
		//MeshComp->SetRelativeLocationAndRotation(FVector(51.0f, -3.0f, 140.0f), FRotator(-32.0f, -39.0f, 24.0f));
		MeshComp->SetRelativeLocationAndRotation(FVector(45.0f, 45.0f, 25.0f), FRotator(-23.0f, -23.0f, 36.0f));
		// Collision 이 영점일 때 Player 가 잡기 위한 수치 : FVector(51.0f, -3.0f, 140.0f), FRotator(-32.0f, -39.0f, 24.0f)
	}
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void APWBlade::BeginPlay()
{
	Super::BeginPlay();

	MaxCombo = 3;
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &APWBlade::OnBoxOverlap);
}

void APWBlade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APWBlade::Attack(AActor* OtherActor)
{
	// 수정해야할 사항 : EnemyInfo 를 상속한 타입인지 확인 할 수 있어야함
	if (OtherActor->GetParentActor()->IsA<AEnemyInfo>()) { UE_LOG(LogTemp, Error, TEXT("Attack to Enemy!"));
		AEnemyInfo* enemy = Cast<AEnemyInfo>(OtherActor);

		// 몇번째 콤보인가에 따라 공격 실행
		switch (player->Combo)
		{
		case 1:
			Strike(enemy);
			break;
		case 2:
			Chop(enemy);
			break;
		case 3:
			Thrust(enemy);
			break;
		case -100: // 대시 공격
			player->Combo = 3;
			Thrust(enemy);
			break;
		default:
			break;
		}
	}
}

void APWBlade::SpecialAtt(AActor* OtherActor)
{
	if (OtherActor->IsA<AEnemyInfo>()) {
		AEnemyInfo* enemy = Cast<AEnemyInfo>(OtherActor);
		NovaSmash(enemy);
	}
}

// 공격에 atkBuff, 영구적 은혜 (있을 시 백어택 확인후 적용) 적용

void APWBlade::Strike(AEnemyInfo* Enemy)
{
	// 20 대미지
}

void APWBlade::Chop(AEnemyInfo* Enemy)
{
	// 25 대미지
}

void APWBlade::Thrust(AEnemyInfo* Enemy)
{
	// 30 대미지

	Knockback(Enemy);
}

void APWBlade::NovaSmash(AEnemyInfo* Enemy)
{
	// 50 대미지

	BackstabBan(Enemy);
	Knockback(Enemy);
}

void APWBlade::Knockback(AEnemyInfo* Enemy)
{
	// 넉백
}

void APWBlade::BackstabBan(AEnemyInfo* Enemy)
{
	// 백스텝 불가
}

void APWBlade::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	switch (player->NowState)
	{
	case EPlayerBehaviorState::Attack:
		Attack(OtherActor);
		break;
	case EPlayerBehaviorState::SpecialAtt:
		SpecialAtt(OtherActor);
		break;
	default:
		break;
	}
}
