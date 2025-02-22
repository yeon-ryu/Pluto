// Fill out your copyright notice in the Description page of Project Settings.


#include "KDestroyBox.h"
#include "KEnemy.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Monster.h"


// Sets default values
AKDestroyBox::AKDestroyBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	RootComponent = BoxComp;
	BoxComp->SetBoxExtent(FVector(500));


	BoxComp->SetGenerateOverlapEvents(true);
	BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	

	
	//범위 확인용 메쉬컴포넌트
	/*
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);



	ConstructorHelpers::FObjectFinder<UStaticMesh>tmpMesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (tmpMesh.Succeeded())
	{
		MeshComp->SetStaticMesh(tmpMesh.Object);

	}
	*/

	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AKDestroyBox::OnBoxOverlap);
}

// Called when the game starts or when spawned
void AKDestroyBox::BeginPlay()
{
	Super::BeginPlay();
	
	//BoxComp->SetRelativeLocation(FVector(2000.f));
}

// Called every frame
void AKDestroyBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bOnSpawn == true)
	{

		curretTime += DeltaTime;

		if (curretTime > 3.0f)
		{
			curretTime = 0.f;

			this->SetActorLocation(FVector(2000));
			bOnSpawn = false;
		}
	}

}

void AKDestroyBox::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//AKEnemy* enemy = Cast<AKEnemy>(OtherActor);
	AMonster* enemy = Cast<AMonster>(OtherActor);


	if (enemy != nullptr)
	{
		enemy->Destroy();
	}

}

void AKDestroyBox::SetbOnSpawn()
{
	bOnSpawn = true;

}

