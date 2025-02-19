// Fill out your copyright notice in the Description page of Project Settings.


#include "PWBlade.h"
#include "EnemyInfo.h"
#include "PlayerZagreus.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

APWBlade::APWBlade()
{
	// 주석 처리 : 충돌을 칼날 부분에 맞도록 했을 경우. 플레이어의 타격감을 위해 충돌 범위를 확 늘림

	CollisionComp->SetBoxExtent(FVector(50.0f, 50.0f, 100.0f));
	//CollisionComp->SetBoxExtent(FVector(10.0f, 10.0f, 60.0f));
	CollisionComp->SetRelativeLocation(FVector(-6.0f, 4.0f, 90.0f));
	//CollisionComp->SetRelativeLocation(FVector(-6.0f, 4.0f, 105.0f));

	// 검 에셋
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BladeMeshComp"));
	MeshComp->SetupAttachment(CollisionComp);

	ConstructorHelpers::FObjectFinder<UStaticMesh>TempMesh(TEXT("/Script/Engine.StaticMesh'/Game/RGY/Modelings/Blade/stygian_blade_-_hades.stygian_blade_-_hades'"));

	if (TempMesh.Succeeded()) {
		MeshComp->SetStaticMesh(TempMesh.Object);
		MeshComp->SetRelativeScale3D(FVector(0.08f));
		MeshComp->SetRelativeLocationAndRotation(FVector(45.0f, 45.0f, 50.0f), FRotator(-23.0f, -23.0f, 36.0f));
		//MeshComp->SetRelativeLocationAndRotation(FVector(45.0f, 45.0f, 25.0f), FRotator(-23.0f, -23.0f, 36.0f));
	}
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 스페셜 어택 범위
	EffectCollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("EffectCollisionComp"));
	EffectCollisionComp->SetupAttachment(RootComp);
	EffectCollisionComp->SetSphereRadius(500.0f);
	EffectCollisionComp->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CollisionComp->SetCollisionObjectType(ECC_Pawn);
	CollisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	EffectCollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	EffectCollisionComp->SetCollisionObjectType(ECC_Pawn);
	EffectCollisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	EffectCollisionComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void APWBlade::BeginPlay()
{
	Super::BeginPlay();

	MaxCombo = 3;
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &APWBlade::OnBoxOverlap);
	EffectCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &APWBlade::OnEffectOverlap);
}

void APWBlade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APWBlade::AttackProcess(AActor* OtherActor)
{
	// EnemyInfo 를 상속한 타입으로 확인 됨. -> EnemyInfo 입장에서 PlayerWeapon overlap 으로 바꾸는거 잊지 말기!!!
	if (OtherActor->IsA<AEnemyInfo>()) {
		UE_LOG(LogTemp, Warning, TEXT("Attack to Enemy!"));
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
		default:
			break;
		}
	}
}

void APWBlade::SpecialAttProcess(AActor* OtherActor)
{
	if (OtherActor->IsA<AEnemyInfo>()) {
		UE_LOG(LogTemp, Warning, TEXT("Special Attack to Enemy!"));
		AEnemyInfo* enemy = Cast<AEnemyInfo>(OtherActor);
		NovaSmash(enemy);
	}
}

void APWBlade::StartAttack()
{
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void APWBlade::EndAttack()
{
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void APWBlade::StartSpecialAtt()
{
	EffectCollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void APWBlade::EndSpecialAtt()
{
	EffectCollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// 공격에 atkBuff, 영구적 은혜 (있을 시 백어택 확인후 적용) 적용

void APWBlade::Strike(AEnemyInfo* Enemy)
{
	// 20 대미지
	int32 damage = 20;

	UGameplayStatics::ApplyDamage(Enemy, damage, GetInstigator()->GetController(), this, UDamageType::StaticClass());
}

void APWBlade::Chop(AEnemyInfo* Enemy)
{
	// 25 대미지
	int32 damage = 25;

	UGameplayStatics::ApplyDamage(Enemy, damage, GetInstigator()->GetController(), this, UDamageType::StaticClass());
}

void APWBlade::Thrust(AEnemyInfo* Enemy)
{
	// 30 대미지
	int32 damage = 30;

	UGameplayStatics::ApplyDamage(Enemy, damage, GetInstigator()->GetController(), this, UDamageType::StaticClass());

	FVector direction = FVector(player->AttackDirection.X, player->AttackDirection.Y, 0.0f);
	Knockback(Enemy, direction);
	player->LaunchCharacter(direction.GetSafeNormal() * 3000, false, false);
}

void APWBlade::NovaSmash(AEnemyInfo* Enemy)
{
	// 50 대미지
	int32 damage = 50;

	UGameplayStatics::ApplyDamage(Enemy, damage, GetInstigator()->GetController(), this, UDamageType::StaticClass());

	FVector direction = Enemy->GetActorLocation() - this->GetActorLocation();
	direction = FVector(direction.X, direction.Y, 0.0f);
	BackstabBan(Enemy);
	Knockback(Enemy, direction);
}

void APWBlade::Knockback(AEnemyInfo* Enemy, FVector dir)
{
	// 넉백
	Enemy->LaunchCharacter(dir.GetSafeNormal() * 3000, false, false);
}

void APWBlade::BackstabBan(AEnemyInfo* Enemy)
{
	// 백스탭 불가 -> 배후 공격 대미지 적용 X
}

void APWBlade::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	switch (player->NowState)
	{
	case EPlayerBehaviorState::Attack:
		AttackProcess(OtherActor);
		break;
	default:
		break;
	}
}

void APWBlade::OnEffectOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	switch (player->NowState)
	{
	case EPlayerBehaviorState::SpecialAtt:
		if(player->bSpecialAtt) SpecialAttProcess(OtherActor);
		break;
	default:
		break;
	}
}
