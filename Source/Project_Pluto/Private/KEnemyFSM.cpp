// Fill out your copyright notice in the Description page of Project Settings.


#include "KEnemyFSM.h"

// Sets default values for this component's properties
UKEnemyFSM::UKEnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UKEnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UKEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UKEnemyFSM::State_Idle()
{
}

void UKEnemyFSM::State_Move()
{
}

void UKEnemyFSM::State_MoveFar()
{

}

void UKEnemyFSM::State_Attack()
{
}

void UKEnemyFSM::State_Damaged()
{
}

void UKEnemyFSM::State_Dead()
{
}

void UKEnemyFSM::OnDamagedProcess()
{
}

void UKEnemyFSM::OnAttackProcess()
{
}

