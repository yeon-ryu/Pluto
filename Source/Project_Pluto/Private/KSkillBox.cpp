// Fill out your copyright notice in the Description page of Project Settings.


#include "KSkillBox.h"
#include "Components/BoxComponent.h"

// Sets default values
AKSkillBox::AKSkillBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	RootComponent = BoxComp;
	BoxComp->SetBoxExtent(FVector(100));


}

// Called when the game starts or when spawned
void AKSkillBox::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AKSkillBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bOnSpawn == true)
	{

		curretTime += DeltaTime;

		if (curretTime > 3.0f)
		{
			curretTime = 0.f;

			this->SetActorLocation(FVector(3000));
			bOnSpawn = false;
		}
	}

}

void AKSkillBox::SetbOnSpawn()
{
	bOnSpawn = true;
}

