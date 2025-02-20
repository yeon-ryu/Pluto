// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerZagreus.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "PWBlade.h"
#include "PlayerAnimInstance.h"

// Sets default values
APlayerZagreus::APlayerZagreus()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 플레이어 메시 에셋 및 초기 위치 각도 설정
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/RGY/Modelings/ParagonGreystone/Characters/Heroes/Greystone/Meshes/Greystone.Greystone'"));

	if (TempMesh.Succeeded()) {
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FRotator(0.0f, -90.0f, 0.0f));
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("PlayerZagreus SkeletalMesh loding fail."));
	}
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 플레이어가 이동 방향으로 회전하도록
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;


	// 애니메이션 클래스 할당
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	ConstructorHelpers::FClassFinder<UAnimInstance> TempAnim(TEXT("/Game/RGY/Blueprints/ABP_PlayerZagreus.ABP_PlayerZagreus_C"));

	if (TempAnim.Succeeded()) {
		GetMesh()->SetAnimInstanceClass(TempAnim.Class);
	}


	// 초기 카메라 설정
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	springArmComp->SetupAttachment(RootComponent);
	springArmComp->SetRelativeLocation(FVector(0.0f));
	springArmComp->TargetArmLength = 1200.0f;
	springArmComp->bDoCollisionTest = false;
	springArmComp->SetWorldRotation(FRotator(-50.0f, -30.0f, 0.0f));

	camComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CamComp"));
	camComp->SetupAttachment(springArmComp);

	GetCapsuleComponent()->SetCollisionObjectType(ECC_Pawn);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

// Called when the game starts or when spawned
void APlayerZagreus::BeginPlay()
{
	Super::BeginPlay();

	// Input 용 컨트롤러 세팅
	pController = Cast<APlayerController>(Controller);
	if (pController) {
		auto subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pController->GetLocalPlayer());

		if (subsystem) {
			subsystem->AddMappingContext(IMC_Player, 0);
		}
	}

	// 무기 소환 세팅
	{
		FName WeaponSocket(TEXT("muzzle_01")); // FX_weapon
		FTransform weaponPosition = GetMesh()->GetSocketTransform(TEXT("muzzle_01"));
		auto CurrentWeapon = GetWorld()->SpawnActor<APWBlade>(FVector::ZeroVector, FRotator::ZeroRotator);
		if (CurrentWeapon != nullptr) {
			weapon = CurrentWeapon;
			weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("FX_weapon"));
		}
	}

	// 애니메이션 Instance 가져오기
	AnimInstance = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());

	// 캐릭터 초기 이동 스피드 세팅
	Speed = RunSpeed;
}

// Called every frame
void APlayerZagreus::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 피격 애니메이션 진행 중
	if(bDamaged) return;

	// 바닥에 검 박고 Q 스킬 어택 ~ 검을 빼고 설 때까지 스스로 아무 것도 하지 못한다.
	if(bSpecialAtt) {
		// 버그났을 경우
		if (NowState != EPlayerBehaviorState::SpecialAtt) {
			// Q 스킬 효과 제거
			weapon->EndSpecialAtt();
			bSpecialAtt = false;
			bForceSpecialAtt = false;
			if (Speed == SpecialAttRunSpeed) {
				Speed = RunSpeed;
			}
		}
		return;
	}

	// 회피 도중 공격 (사람 인식으로)씹힘이 잦아 공격 인풋 더 기다려 줌
	if (bDodgeAttackWait) {
		CheckDodgeAttackInput(DeltaTime);
	}

	// 회피 후 연속 회피 안되도록 Delay 타임 체크
	if (bDodgeDelayWait) {
		CheckDodgeDelay(DeltaTime);
	}

	// 플레이어가 Move 상태인데 움직임이 없을 경우 Idle 상태로 변경
	if (PlayerDir == FVector::ZeroVector && NowState == EPlayerBehaviorState::Move) {
		NowState = EPlayerBehaviorState::Idle;
	}

	// 플레이어 이동
	{
		if(NowState == EPlayerBehaviorState::Move || NowState == EPlayerBehaviorState::Dodge || NowState == EPlayerBehaviorState::SpecialAtt) {
			if (PlayerDir == FVector::ZeroVector) {
				if (pController != nullptr) {
					PlayerDir = pController->GetPawn()->GetActorForwardVector();
				}
			}

			if (NowState != EPlayerBehaviorState::SpecialAtt && Speed == SpecialAttRunSpeed) {
				if(NowState == EPlayerBehaviorState::Dodge) Speed = DodgeSpeed;
				else Speed = RunSpeed;
			}

			SetActorRotation(FRotator(0.0f, UKismetMathLibrary::ClampAxis(PlayerDir.Rotation().Yaw), 0.0f));

			SetActorLocation(GetActorLocation() + PlayerDir.GetSafeNormal() * Speed * DeltaTime, true);

			PlayerDir = FVector::ZeroVector;
		}
	}

	{
		if (NowState == EPlayerBehaviorState::Dodge) { // 우선순위 최강 회피
			CurrentAnimTime += DeltaTime;
			if (CurrentAnimTime >= AnimWaitTime) {
				CurrentAnimTime = 0.0f;
				EndDodge();
			}
		}
		else {
			if (bAttackProcess) { // 공격 중이면 공격 애니메이션 대기
				// GEngine->AddOnScreenDebugMessage(0, 1, FColor::Green, FString::Printf(TEXT("Combo : %d"), Combo));

				CurrentAnimTime += DeltaTime;
				if (CurrentAnimTime >= AnimWaitTime) {
					CurrentAnimTime = 0.0f;
					AttackProcess();
				}
			}
			else if (bReserveAttack) { // 공격 중이 아니고 공격 예약이 결려있으면 공격 프로세스 진행
				weapon->StartAttack();
				AttackProcess();
			}
		}
	}

	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Green, FString::Printf(TEXT("Player State : %s"), *UEnum::GetValueAsString(NowState)));
	GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::Red, FString::Printf(TEXT("Player Collision : %s"), *UEnum::GetValueAsString(GetCapsuleComponent()->GetCollisionResponseToChannel(ECC_Pawn))));
}

// Called to bind functionality to input
void APlayerZagreus::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto PlayerInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (PlayerInput) {
		PlayerInput->BindAction(IA_Move, ETriggerEvent::Triggered, this, &APlayerZagreus::Move);
		PlayerInput->BindAction(IA_Attack, ETriggerEvent::Started, this, &APlayerZagreus::Attack);
		PlayerInput->BindAction(IA_Dodge, ETriggerEvent::Started, this, &APlayerZagreus::Dodge);
		PlayerInput->BindAction(IA_SpecialAtt, ETriggerEvent::Started, this, &APlayerZagreus::SpecialAtt);
		//PlayerInput->BindAction(IA_Spell, ETriggerEvent::Started, this, &APlayerZagreus::Spell);
		//PlayerInput->BindAction(IA_Interaction, ETriggerEvent::Started, this, &APlayerZagreus::Interaction);
		
		PlayerInput->BindAction(IA_CheatInvincible, ETriggerEvent::Started, this, &APlayerZagreus::CheatInvincible);
	}
}

void APlayerZagreus::NotifyActorBeginOverlap(AActor* OtherActor)
{
	// 돌진 은혜 시 적과 부딪히면 적한테 대미지

}

void APlayerZagreus::CheckDodgeDelay(float DeltaTime)
{
	CurrentDodgeDelayWait += DeltaTime;

	if (CurrentDodgeDelayWait >= DodgeDelayTime) {
		bDodgeDelayWait = false;
		CurrentDodgeDelayWait = 0.0f;
	}
}

void APlayerZagreus::CheckDodgeAttackInput(float DeltaTime)
{
	CurrentDodgeAttackWait += DeltaTime;

	if ((NowState != EPlayerBehaviorState::Idle && NowState != EPlayerBehaviorState::Move) || CurrentDodgeAttackWait >= DodgeAttackTime) {
		bDodgeAttackWait = false;
		CurrentDodgeAttackWait = 0.0f;
		return;
	}

	if (bReserveAttack) { // 이 부분 언젠가 확장 가능하게 대시 스트라이크 전용코드로 변경
		Combo = weapon->MaxCombo - 1;
		bDodgeAttackWait = false;
		CurrentDodgeAttackWait = 0.0f;
		AttackProcess();
	}
}

void APlayerZagreus::EndDodge()
{
	Speed = RunSpeed;
	// 에너미와 충돌 Overlap 으로 원복
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	if (NowState == EPlayerBehaviorState::Dodge) {
		NowState = EPlayerBehaviorState::Idle;
	}

	AnimWaitTime = DefaultAnimWaitTime;

	bDodgeAttackWait = true;
	CurrentDodgeAttackWait = 0.0f;

	bDodgeDelayWait = true;
	CurrentDodgeDelayWait = 0.0f;

	if (bForceSpecialAtt) {
		StartSpecialAtt();
		return;
	}

	if (bReserveAttack) { // 이 부분 언젠가 확장 가능하게 대시 스트라이크 전용코드로 변경
		Combo = weapon->MaxCombo - 1;
		AttackProcess();
	}
}

void APlayerZagreus::SetAttackDir()
{
	auto PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController) {
		FHitResult hit;
		PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, hit);

		MouseLocation = hit.Location;

		AttackDirection = MouseLocation - GetActorLocation();
		AttackDirection = FVector(AttackDirection.X, AttackDirection.Y, 0.0f); // Z 축 값이 혹여나 안 들어오게 처리
		SetActorRotation(FRotator(0.0f, UKismetMathLibrary::ClampAxis(AttackDirection.Rotation().Yaw), 0.0f));
	}
}

void APlayerZagreus::AttackProcess()
{
	if(bSpecialAtt || !CheckChangeStateEnabled(EPlayerBehaviorState::Attack)) return;
	Speed = RunSpeed;

	if(!bAttackProcess) { // 어택 시작
	
		bAttackProcess = true;
		bReserveAttack = false;
		SetAttackDir();

		Combo++;
		CurrentAnimTime = 0.0f;

		if (Combo > weapon->MaxCombo) {
			Combo = 1;
		}

		if (NowState != EPlayerBehaviorState::Attack) {
			NowState = EPlayerBehaviorState::Attack;
		}
	}
	else { // 어택 애니메이션 종료 후 로직
		if (!bReserveAttack) {
			Combo = 0;
			if (NowState == EPlayerBehaviorState::Attack) {
				NowState = EPlayerBehaviorState::Idle;
			}
			weapon->EndAttack();
		}
		
		bAttackProcess = false;
	}
}

float APlayerZagreus::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if(HP == 0) return 0;

	if (GetCapsuleComponent()->GetCollisionResponseToChannel(ECC_Pawn) == ECR_Ignore) {
		return HP;
	}

	HP = FMath::Clamp(HP-Damage, 0.f, (float)(MaxHP));

	if(NowState != EPlayerBehaviorState::Damaged && CheckChangeStateEnabled(EPlayerBehaviorState::Damaged)) {
		NowState = EPlayerBehaviorState::Damaged;
	}

	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, FString::Printf(TEXT("Player HP : %d"), HP));

	return HP;
}

void APlayerZagreus::StartSpecialAtt()
{
	if (NowState != EPlayerBehaviorState::SpecialAtt && CheckChangeStateEnabled(EPlayerBehaviorState::SpecialAtt)) {
		NowState = EPlayerBehaviorState::SpecialAtt;
		Speed = SpecialAttRunSpeed;
		bForceSpecialAtt = false;
		bSpecialAtt = false;
	}
}

void APlayerZagreus::SetPermanetBuff()
{
	MaxHP = InitHP + FMath::Floor(InitHP * PlusHP);
}

void APlayerZagreus::SetBuffMaxHP(int32 plusHpAbs, float plusHpPro)
{
	// 현재 HP 도 최대 HP 변경에 따라 변함. 실제로 어떤지 게임 확인 필요

	MaxHP += plusHpAbs;
	HP += plusHpAbs;

	MaxHP += FMath::Floor(MaxHP * plusHpPro);
	HP += FMath::Floor(HP * plusHpPro);
}

bool APlayerZagreus::CheckChangeStateEnabled(EPlayerBehaviorState state)
{
	if(NowState == state) return true;

	switch (NowState)
	{
	case EPlayerBehaviorState::Idle:
		if(state != EPlayerBehaviorState::Spawn && state != EPlayerBehaviorState::Die)
			return true;
		break;
	case EPlayerBehaviorState::Move:
		if (state != EPlayerBehaviorState::Spawn && state != EPlayerBehaviorState::Die)
			return true;
		break;
	case EPlayerBehaviorState::Attack:
		if(state == EPlayerBehaviorState::Idle || state == EPlayerBehaviorState::Dodge || state == EPlayerBehaviorState::Damaged)
			return true;
		break;
	case EPlayerBehaviorState::Dodge:
		if(state == EPlayerBehaviorState::Idle || state == EPlayerBehaviorState::Attack || state == EPlayerBehaviorState::SpecialAtt)
			return true;
		break;
	case EPlayerBehaviorState::SpecialAtt:
		if(state == EPlayerBehaviorState::Idle || state == EPlayerBehaviorState::Attack || state == EPlayerBehaviorState::Dodge || state == EPlayerBehaviorState::Damaged)
			return true;
		break;
	case EPlayerBehaviorState::Spell:
		break;
	case EPlayerBehaviorState::Interaction:
		break;
	case EPlayerBehaviorState::Damaged:
		if(state == EPlayerBehaviorState::Idle || state == EPlayerBehaviorState::Die)
			return true;
		break;
	case EPlayerBehaviorState::Die:
		break;
	case EPlayerBehaviorState::Spawn:
		if(state == EPlayerBehaviorState::Idle)
			return true;
		break;
	default:
		break;
	}
	return false;
}

void APlayerZagreus::Move(const FInputActionValue& inputValue)
{
	if(bDamaged || bSpecialAtt) return;

	//if(NowState != EPlayerBehaviorState::Idle && NowState != EPlayerBehaviorState::Move) {
	//	if (PlayerDir == FVector::ZeroVector) {
	//		FVector2D value = inputValue.Get<FVector2D>();
	//		PlayerDir.X = value.X;
	//		PlayerDir.Y = value.Y;
	//	}
	//	return;
	//}

	if (NowState != EPlayerBehaviorState::Move && CheckChangeStateEnabled(EPlayerBehaviorState::Move)) {
		NowState = EPlayerBehaviorState::Move;
	}

	FVector2D value = inputValue.Get<FVector2D>();
	PlayerDir.X = value.X;
	PlayerDir.Y = value.Y;
}

// 에너미 오버랩 시 공격은 무기에서
void APlayerZagreus::Attack(const FInputActionValue& inputValue)
{
	if(bDamaged || bSpecialAtt || !CheckChangeStateEnabled(EPlayerBehaviorState::Attack)) return;
	bReserveAttack = true;
	Speed = RunSpeed;
	bForceSpecialAtt = false;
}

void APlayerZagreus::Dodge(const FInputActionValue& inputValue)
{
	// 피격 상태 / 회피 재사용 대기 시간 / Q 스킬 공격 중 / 대시 도중 및 대시 불가능한 상태일 경우
	if(bDamaged || bDodgeDelayWait || bSpecialAtt || NowState == EPlayerBehaviorState::Dodge || !CheckChangeStateEnabled(EPlayerBehaviorState::Dodge)) return;

	if (NowState == EPlayerBehaviorState::SpecialAtt) {
		if (bForceSpecialAtt) {
			return;
		}
		else {
			bForceSpecialAtt = true;
		}
	}

	NowState = EPlayerBehaviorState::Dodge;

	if (bAttackProcess) {
		bAttackProcess = false;
		bReserveAttack = false;
		Combo = 0;
		CurrentAnimTime = 0.0f;
		weapon->EndAttack();
	}

	Speed = DodgeSpeed;
	// 에너미와 충돌 Ignore
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	bDodgeAttackWait = false;
	AnimWaitTime = DodgeTime;
}

// 에너미 오버랩 시 공격은 무기에서
void APlayerZagreus::SpecialAtt(const FInputActionValue& inputValue)
{
	if(bDamaged) return;
	StartSpecialAtt();
}

void APlayerZagreus::Spell(const FInputActionValue& inputValue)
{
	if (NowState != EPlayerBehaviorState::Spell) {
		NowState = EPlayerBehaviorState::Spell;
	}
	SetAttackDir();

	// 마법 애니메이션 -> 끝나면 NowState 변경
}

void APlayerZagreus::Interaction(const FInputActionValue& inputValue)
{
	if (NowState != EPlayerBehaviorState::Interaction) {
		NowState = EPlayerBehaviorState::Interaction;
	}

	// 상호작용
}

void APlayerZagreus::CheatInvincible(const FInputActionValue& inputValue)
{
	if (GetCapsuleComponent()->GetCollisionResponseToChannel(ECC_Pawn) == ECR_Ignore) {
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	}
	else {
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	}
}
