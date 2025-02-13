// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputActionValue.h"
#include "PlayerZagreus.generated.h"

UENUM(BlueprintType)
enum class EPlayerBehaviorState : uint8 {
	Idle UMETA(DisplayName="Idle"),
	Move UMETA(DisplayName="Move(WASD)"),
	Attack UMETA(DisplayName="Attack(Left Click)"),
	Dodge UMETA(DisplayName="Dodge(Space)"),
	SpecialAtt UMETA(DisplayName="SpecialAtt(Q)"),
	Spell UMETA(DisplayName="Spell(Right Click)"),
	Interaction UMETA(DisplayName="Interaction(E)")
};

UENUM(BlueprintType)
enum class EPlayerWeapon : uint8 {
	Blade UMETA(DisplayName = "Blade")
};

UCLASS()
class PROJECT_PLUTO_API APlayerZagreus : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerZagreus();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


// 플레이어 설정
public:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	UPROPERTY(EditAnywhere, Category = Camera)
	class USpringArmComponent* springArmComp;

	UPROPERTY(EditAnywhere, Category = Camera)
	class UCameraComponent* camComp;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerState)
	EPlayerBehaviorState NowState = EPlayerBehaviorState::Idle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerState)
	EPlayerWeapon NowWeapon = EPlayerWeapon::Blade;


	UPROPERTY(EditAnywhere, Category="PlayerSetting")
	float Speed = 700.0f;

	UPROPERTY(EditAnywhere, Category="PlayerSetting")
	float DodgeSpeed = 4000.0f;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Weapon)
	class APlayerWeapon* weapon;

	// void ChangeWeapon();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerSetting")
	int32 InitHP = 50; // 플레이어 초기 체력

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerSetting")
	int32 MaxHP = 50; // 플레이어 최대 체력

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PlayerSetting")
	int32 HP = 50; // 플레이어 현재 체력


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PlayerSetting")
	float PlusHP = 0; // 영구 강화 : 추가 HP 퍼센트

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PlayerSetting")
	float PlusAtk = 0; // 영구 강화 : 추가 공격력 퍼센트


	UPROPERTY()
	class UPlayerAnimInstance* AnimInstance;


// 플레이어 로직
public:
	class APlayerController* pController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim")
	float AnimWaitTime = 0.6f; // 애니메이션 대기 시간. Montage_Play 로 애니메이션 시간 세팅

	float CurrentAnimTime = 0.0f;

	float DefaultAnimWaitTime = 0.6f;

	float DodgeTime = 0.5f;


	FVector PlayerDir;


	FVector AttackDirection; // 공격 방향 (플레이어 기준 마우스 방향)

	FVector MouseLocation; // 마우스 위치 (입력 받은 시점 마우스 위치)

	// 공격할 때 마우스 위치 및 방향 구하기
	void SetAttackDir();


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combo")
	int32 Combo = 0; // 몇번째 콤보인가

	bool bAttackProcess = false; // 어택 중인가
	
	bool bReserveAttack = false; // 어택 예약

	void AttackProcess(); // 어택 로직


	void EndDodge();


	// 피격
	void OnDamage(int32 damage);

	void SetPermanetBuff();

	// hp 최대치 변경 아이템 먹었을 경우 / 인자값 : 절대값 플러스, 퍼센트 플러스
	void SetBuffMaxHP(int32 plusHpAbs, float plusHpPro);


// Input
public:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext* IMC_Player;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Move;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Attack;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Dodge;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_SpecialAtt;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Spell;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Interaction;


	void Move(const FInputActionValue& inputValue); // 이동
	void Attack(const FInputActionValue& inputValue); // 일반 공격 & 콤보
	void Dodge(const FInputActionValue& inputValue); // 회피 & 대시
	void SpecialAtt(const FInputActionValue& inputValue); // 특수 공격 (Q 스킬)
	void Spell(const FInputActionValue& inputValue); // 마법 (우클릭)
	void Interaction(const FInputActionValue& inputValue); // 상호작용 (E 버튼)
};
