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

	// ¹«±â collision
	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("RootCollisionComp"));
	CollisionComp->SetupAttachment(CollisionComp);
}

// Called when the game starts or when spawned
void APlayerWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerZagreus::StaticClass());
	player = Cast<APlayerZagreus>(actor);
}

// Called every frame
void APlayerWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerWeapon::Attack(AActor* OtherActor)
{
}

void APlayerWeapon::SpecialAtt(AActor* OtherActor)
{
}

void APlayerWeapon::NotifyActorBeginOverlap(AActor* OtherActor)
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

