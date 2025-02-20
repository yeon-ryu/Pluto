// Fill out your copyright notice in the Description page of Project Settings.


#include "PlateActor.h"
#include "Components/SceneComponent.h"
#include "DebugMacro.h"
#include "../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h"
#include "../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerZagreus.h"
#include "Boss.h"
#include "EnemyInfo.h"
#include "EngineUtils.h"


// Sets default values
APlateActor::APlateActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	SetRootComponent(SceneComp);

	ConstructorHelpers::FObjectFinder<UNiagaraSystem> NS(TEXT("/Script/Niagara.NiagaraSystem'/Game/MagicCircleVFX/VFX/NS_TP-Circle_22.NS_TP-Circle_22'"));
	if (NS.Succeeded())
	{
		SpawnFXSystem = NS.Object;
	}

	ConstructorHelpers::FObjectFinder<UNiagaraSystem> TempNs(TEXT("/Script/Niagara.NiagaraSystem'/Game/MW/StylizedNiagara/Particles/Explosion_01/NS_Explosion_01.NS_Explosion_01'"));
	if (TempNs.Succeeded())
	{
		ExplosionFXSystem = TempNs.Object;
	}
	
}

// Called when the game starts or when spawned
void APlateActor::BeginPlay()
{
	Super::BeginPlay();

	player = Cast<APlayerZagreus>(GetWorld()->GetFirstPlayerController()->GetPawn());
	boss = Cast<ABoss>(GetOwner());

	SpawnFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), SpawnFXSystem, GetActorLocation(), FRotator::ZeroRotator, FVector(5.f, 5.f, 1.f), true, true);
	
	SpawnFXComponent->Activate();

	GetWorld()->GetTimerManager().SetTimer(ExplosionTimerHandler, this, &APlateActor::ActivateExplosion, 1.f, false, 1.f);

	
}

// Called every frame
void APlateActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);



}



void APlateActor::ActivateExplosion()
{
	GetWorldTimerManager().ClearTimer(ExplosionTimerHandler);

	ExplosionFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionFXSystem, GetActorLocation(), FRotator(0.f), FVector(.5f), true, true);

	ExplosionFXComponent->Activate();

	TArray<AActor*> IgnoreActors;
	for (TActorIterator<AEnemyInfo> It(GetWorld()); It; ++It)
	{
		IgnoreActors.Add(*It);
	}
	

	if (player != nullptr && boss != nullptr)
	{
		FVector vc = player->GetActorLocation() - this->GetActorLocation();
		float dist = player->GetDistanceTo(this);
		if (dist <= 400.f)
		{
			UGameplayStatics::ApplyRadialDamage(GetWorld (), boss->GetDamage(), this->GetActorLocation(),
				400.f, UDamageType::StaticClass(), IgnoreActors, boss, boss->GetInstigatorController(), true);
			
			UE_LOG(LogTemp, Error, TEXT("Plate hit %s! Applied %.2f Damage!"), *player->GetName(), boss->GetDamage());

		}

	}



	SpawnFXComponent->Deactivate();
	SpawnFXComponent->DestroyComponent();

	GetWorld()->GetTimerManager().SetTimer(DeactivateTimerHandler, this, &APlateActor::DeactivateComponent, 1.f, false, 1.5f);

}

void APlateActor::DeactivateComponent()
{
	
	GetWorldTimerManager().ClearTimer(DeactivateTimerHandler);
	ExplosionFXComponent->Deactivate();


	this->Destroy();
}


