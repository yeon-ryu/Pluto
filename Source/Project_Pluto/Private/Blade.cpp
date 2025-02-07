// Fill out your copyright notice in the Description page of Project Settings.


#include "Blade.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerZagreus.h"

// Sets default values
ABlade::ABlade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABlade::BeginPlay()
{
	Super::BeginPlay();
	
	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerZagreus::StaticClass());
	player = Cast<APlayerZagreus>(actor);
}

// Called every frame
void ABlade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// 공격에 FMath::Floor(데미지 + FMath::Floor(데미지 * PlusAtk * atkBuff)) 적용

void ABlade::Strike()
{
	// 20 대미지
}

void ABlade::Chop()
{
	// 25 대미지
}

void ABlade::Thrust()
{
	// 30 대미지

	Knockback();
}

void ABlade::NovaSmash()
{
	// 50 대미지
	
	BackstabBan();
	Knockback();
}

void ABlade::Knockback()
{
	// 넉백
}

void ABlade::BackstabBan()
{
	// 백스텝 불가
}

