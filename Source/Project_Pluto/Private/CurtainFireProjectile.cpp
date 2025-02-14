// Fill out your copyright notice in the Description page of Project Settings.


#include "CurtainFireProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

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

