// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MonsterFSM.generated.h"

//사용할 상태 정의
UENUM(BlueprintType)
enum class EMonsterState : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Move UMETA(DisplayName = "Move"),
	Attack_Start		UMETA(DisplayName = "Attack_Start"),
	Attack_End		UMETA(DisplayName = "Attack_End"),
	Damage UMETA(DisplayName = "Damage"),
	Die UMETA(DisplayName = "Die")
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_PLUTO_API UMonsterFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMonsterFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 상태 변수
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "FSM")
	EMonsterState mState = EMonsterState::Idle;

#pragma region State Function

	//대기 상태
	void State_Idle();

	//이동 상태
	void State_Move();

	void State_Attack_Start();

	void State_Attack_End();

	//피격 상태
	void State_Damage();

	//죽음 상태
	void State_Die();

#pragma endregion State Function

	UPROPERTY(EditDefaultsOnly, Category = "FSM")
	float IdleDelayTime = 2.0f;

	// 경과 시간
	float currentTime = 0.0f;

	UPROPERTY(EditAnywhere, Category = "FSM")
	float attackDelayTime = 4.f;

	UPROPERTY(EditAnywhere, Category = "FSM")
	float damageDelayTime = 1.f;

	UPROPERTY(EditAnywhere, Category = "FSM")
	float disapperSpeed = 50.f;

	UPROPERTY(VisibleAnywhere, Category = "FSM")
	class APlayerZagreus* target;

	UPROPERTY()
	class AMonster* me;

	void OnDamageProcess();

	bool bDieDone = false;
	
	FORCEINLINE void OnDieEnd(){ bDieDone = true; };

	UPROPERTY()
	class UMonsterAnimInstance* Anim;

	UPROPERTY()
	class AAIController* AI;

	FVector randomPos;

	bool GetRandomPosInNavMesh(FVector centerLocation, float radius, FVector& dest);
		
};
