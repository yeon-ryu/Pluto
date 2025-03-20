// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterSpawner.h"
#include "../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h"
#include "../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraComponent.h"
#include "Monster.h"

// Sets default values
AMonsterSpawner::AMonsterSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	SetRootComponent(SceneComp);

	ConstructorHelpers::FObjectFinder<UNiagaraSystem> NS(TEXT("/Script/Niagara.NiagaraSystem'/Game/MagicCircleVFX/VFX/NS_TP-Circle_18.NS_TP-Circle_18'"));
	if (NS.Succeeded())
	{
		SpawnFXSystem = NS.Object;
	}

}

// Called when the game starts or when spawned
void AMonsterSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), SpawnFXSystem, GetActorLocation(), FRotator::ZeroRotator, FVector(1.f), true, false);

}

// Called every frame
void AMonsterSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void AMonsterSpawner::NiagaraCaller()
{
	GetWorld ()->GetTimerManager ().SetTimer(NiagaraTimer, this, &AMonsterSpawner::ActivateNiagara, 1.0f, false, 0.5f);

}

void AMonsterSpawner::ActivateNiagara()
{
	SpawnFXComponent->SetHiddenInGame (false);
	SpawnFXComponent->Activate();
	GetWorld ()->GetTimerManager ().SetTimer(SpawnTimer, this, &AMonsterSpawner::SpawnMonster, 1.0f, false, 1.0f);

	

}

void AMonsterSpawner::SpawnMonster()
{
	SpawnFXComponent->SetHiddenInGame (true);
	GetWorldTimerManager ().ClearTimer (NiagaraTimer);

	GetWorld()->SpawnActor <AMonster>(MonsterFactory, this->GetActorLocation(), FRotator(0.f));

	spawnCount++;

	if (spawnCount >= 1)
	{
		spawnCount = 0;
		GetWorldTimerManager ().ClearTimer (SpawnTimer);
	}

}

