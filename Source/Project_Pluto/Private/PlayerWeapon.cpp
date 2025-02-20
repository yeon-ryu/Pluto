// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerZagreus.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/BoxComponent.h"

// Sets default values
APlayerWeapon::APlayerWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);

	// 무기 collision
	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComp"));
	CollisionComp->SetupAttachment(RootComp);

	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionComp->SetCollisionObjectType(ECC_Pawn);
	CollisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); // 나중에 적으로 변경 & 벽도 필요?
}

// Called when the game starts or when spawned
void APlayerWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerZagreus::StaticClass());
	player = Cast<APlayerZagreus>(actor);

	SetOwner(player);
	SetInstigator(player);
}

// Called every frame
void APlayerWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerWeapon::AttackProcess(AActor* OtherActor)
{
}

void APlayerWeapon::SpecialAttProcess(AActor* OtherActor)
{
}

void APlayerWeapon::StartAttack()
{
}

void APlayerWeapon::EndAttack()
{
}

void APlayerWeapon::StartSpecialAtt()
{
}

void APlayerWeapon::EndSpecialAtt()
{
}

void APlayerWeapon::LastCombo()
{

}

