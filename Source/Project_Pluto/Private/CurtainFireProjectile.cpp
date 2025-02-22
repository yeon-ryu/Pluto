// Fill out your copyright notice in the Description page of Project Settings.


#include "CurtainFireProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "PlayerZagreus.h"
#include "EnemyInfo.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACurtainFireProjectile::ACurtainFireProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	//콜리전 생성
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SetRootComponent(SphereComp);


	//Mesh 동적 할당
	MeshComp = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);

	//ProjectileMovement 부착
	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	Movement->ProjectileGravityScale = 0.0f;
	Movement->UpdatedComponent = RootComponent;
	
}

// Called when the game starts or when spawned
void ACurtainFireProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	SetLifeSpan(LifeTime);

	enemy = Cast<AEnemyInfo>(GetOwner());
	// 충돌 이벤트 등록
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ACurtainFireProjectile::OnProjectileOverlap);

}

// Called every frame
void ACurtainFireProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACurtainFireProjectile::SetVelocity(FVector dir_Value)
{
	Movement->Velocity = dir_Value * Speed;
}

void ACurtainFireProjectile::OnProjectileOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	player = Cast<APlayerZagreus>(OtherActor);

	if (player != nullptr && enemy != nullptr)
	{
		UGameplayStatics::ApplyDamage(player, enemy->GetDamage(), enemy->GetInstigatorController(), enemy, UDamageType::StaticClass());

		UE_LOG(LogTemp, Warning, TEXT("Projectile hit %s! Applied %.2f Damage!"), *player->GetName(), enemy->GetDamage());
		
		this->Destroy();
	}


}

