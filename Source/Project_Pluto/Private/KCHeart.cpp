// Fill out your copyright notice in the Description page of Project Settings.


#include "KCHeart.h"
#include "Components/BoxComponent.h"
#include "PlayerZagreus.h"
#include "Engine/SkeletalMesh.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
AKCHeart::AKCHeart()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	RootComponent = BoxComp;
	BoxComp->SetBoxExtent(FVector(50));


	BoxComp->SetGenerateOverlapEvents(true);
	BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AKCHeart::OnBoxOverlap);


	/*
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/KJY/RSS/ParagonSevarog/Characters/Heroes/Sevarog/Meshes/Sevarog.Sevarog'"));


	if (TempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -70.0f), FRotator(0.0f, -90.0f, 0.0f));
		GetMesh()->SetRelativeScale3D(FVector(1.0f));
	}
	*/

	//player = Cast<APlayerZagreus>(GetWorld()->GetFirstPlayerController()->GetPawn());



}

// Called when the game starts or when spawned
void AKCHeart::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AKCHeart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AKCHeart::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerZagreus* auto_player = Cast<APlayerZagreus>(OtherActor);

	if(auto_player == nullptr) return;
	
	//자그레우스 최대 체력 증가
	auto_player->SetBuffMaxHP(25, 0.0f);

	//UE_LOG(LogTemp, Warning, TEXT("Destroy"));

	this->Destroy();

}

