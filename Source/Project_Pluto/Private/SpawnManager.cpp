// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnManager.h"
#include "Kismet/GameplayStatics.h"
#include "Monster.h"
#include "EngineUtils.h"
#include "Boss.h"

// Sets default values
ASpawnManager::ASpawnManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpawnManager::BeginPlay()
{
	Super::BeginPlay();

	ABoss* boss = Cast<ABoss>(UGameplayStatics::GetActorOfClass(GetWorld(), ABoss::StaticClass()));

	if (boss)
	{
		bSpawnable = false;
	}



	FindSpawnPoints ();
	SpawnerCaller ();
	
}

// Called every frame
void ASpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<AActor*> FoundMonsters;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMonster::StaticClass(), FoundMonsters);

	monsterCount = FoundMonsters.Num();

	if ( bSpawnable && monsterCount == 0)
	{
		SpawnerCaller ();
	}

}

void ASpawnManager::WhenMonsterDestroyed()
{
	monsterCount--;

	if (bSpawnable && monsterCount == 0)
	{
		SpawnerCaller ();

	}
}

void ASpawnManager::SpawnerCaller()
{
	if(bSpawnable && spawnLoop >= maxSpawnLoop) return;

	nowTime += GetWorld()->DeltaTimeSeconds;
	if (nowTime > 1.f)
	{
		for (int32 i = 0; i < 6; i++)
		{
			int32 index = FMath::RandRange(0, spawnPoints.Num() - 1);
			spawnPoints[index]->NiagaraCaller();

		}
		spawnLoop++;
		nowTime = 0.f;
	}
}

void ASpawnManager::FindSpawnPoints()
{
	for (TActorIterator<AActor> iter(GetWorld()); iter; ++iter)
	{
		AActor* spawn = *iter;
		//찾은 해당 액터의 이름에서 해당 문자열을 포함하고 있다면,
		if ( spawn->GetName().Contains( TEXT ("BP_EnemySpawnPoint" ) ))
		{
			AMonsterSpawner* points = Cast<AMonsterSpawner>(spawn);
			//스폰 목록에 추가
			spawnPoints.Add(points);
		}
	}
}

