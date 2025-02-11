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

	// 애니메이션 클래스 할당
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	ConstructorHelpers::FClassFinder<UAnimInstance> TempAnim(TEXT("/Game/RGY/Blueprints/ABP_PlayerZagreus.ABP_PlayerZagreus_C"));

	if (TempAnim.Succeeded()) {
		GetMesh()->SetAnimInstanceClass(TempAnim.Class);
		
	}

	// 플레이어가 이동 방향으로 회전하도록
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	if (MovementComponent)
	{
		MovementComponent->bOrientRotationToMovement = true;
		MovementComponent->bUseControllerDesiredRotation = false;
	}

	GetCharacterMovement()->MaxWalkSpeed = Speed;
	GetCharacterMovement()->MaxWalkSpeedCrouched = Speed;

	// 초기 카메라 설정
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	springArmComp->SetupAttachment(RootComponent);
	springArmComp->SetRelativeLocation(FVector(0.0f));
	springArmComp->TargetArmLength = 1000.0f;
	springArmComp->bDoCollisionTest = false;
	springArmComp->SetWorldRotation(FRotator(-50.0f, -30.0f, 0.0f));

	camComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CamComp"));
	camComp->SetupAttachment(springArmComp);

	//weapon = CreateDefaultSubobject<APWBlade>(TEXT("PlayerWeapon"));
	//weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("FX_weapon"));
}

// Called when the game starts or when spawned
void APlayerZagreus::BeginPlay()
{
	Super::BeginPlay();
	
	auto pc = Cast<APlayerController>(Controller);
	if (pc) {
		auto subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());

		if (subsystem) {
			subsystem->AddMappingContext(IMC_Player, 0);
		}
	}

	{
		// 무기 세팅
		FName WeaponSocket(TEXT("muzzle_01")); // FX_weapon
		FTransform weaponPosition = GetMesh()->GetSocketTransform(TEXT("muzzle_01"));
		auto CurrentWeapon = GetWorld()->SpawnActor<APWBlade>(FVector::ZeroVector, FRotator::ZeroRotator);
		if (CurrentWeapon != nullptr) {
			weapon = CurrentWeapon;
			weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("FX_weapon"));
		}
	}
}

// Called every frame
void APlayerZagreus::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (PlayerDir == FVector::ZeroVector && NowState == EPlayerBehaviorState::Move) {
		NowState = EPlayerBehaviorState::Idle;
	}

	GEngine->AddOnScreenDebugMessage(0, 1, FColor::Green, UEnum::GetValueAsString(NowState));

	{
		// 플레이어 이동
		//PlayerDir.Normalize();
		//SetActorLocation(GetActorLocation() + PlayerDir * Speed * DeltaTime);
		AddMovementInput(PlayerDir.GetSafeNormal());
		PlayerDir = FVector::ZeroVector;
	}

	{
		// 콤보 공격 확인
		if (isCombo && NowState == EPlayerBehaviorState::Attack) {
			CurrentAttackTime += DeltaTime;
			if (CurrentAttackTime >= ComboWaitTime) {
				isCombo = false;
				// 공격 시작 시 초기화 해주지만 혹시 모르니 여기서도 콤보 초기화
				Combo = 0;
				CurrentAttackTime = 0.0f;
				
				NowState = EPlayerBehaviorState::Idle;
			}
		}
	}
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
		PlayerInput->BindAction(IA_Spell, ETriggerEvent::Started, this, &APlayerZagreus::Spell);
		PlayerInput->BindAction(IA_Interaction, ETriggerEvent::Started, this, &APlayerZagreus::Interaction);
	}
}

void APlayerZagreus::NotifyActorBeginOverlap(AActor* OtherActor)
{
	// 돌진 은혜 시 적과 부딪히면 적한테 대미지

}

void APlayerZagreus::SetAttackDir()
{
	auto PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController) {
		FHitResult hit;
		PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, hit);

		MouseLocation = hit.Location;

		AttackDirection = MouseLocation - GetActorLocation();
		SetActorRotation(FRotator(0.0f, UKismetMathLibrary::ClampAxis(AttackDirection.Rotation().Yaw), 0.0f));
	}
}

void APlayerZagreus::OnDamage(int32 damage)
{
	HP -= damage;

	// 피격 애니메이션
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

void APlayerZagreus::Move(const FInputActionValue& inputValue)
{
	if (NowState != EPlayerBehaviorState::Move) {
		NowState = EPlayerBehaviorState::Move;
	}

	FVector2D value = inputValue.Get<FVector2D>();
	PlayerDir.X = value.X;
	PlayerDir.Y = value.Y;
}

// 에너미 오버랩 시 공격은 무기에서
void APlayerZagreus::Attack(const FInputActionValue& inputValue)
{
	SetAttackDir();
	UE_LOG(LogTemp, Warning, TEXT("Player : %.1f , %.1f"), GetActorLocation().X, GetActorLocation().Y);
	UE_LOG(LogTemp, Warning, TEXT("Mouse : %.1f , %.1f"), MouseLocation.X, MouseLocation.Y);

	if (!isCombo) { // 콤보 시작 플래그 설정
		Combo = 0;
		isCombo = true;
	}

	Combo++;
	CurrentAttackTime = 0.0f;

	if (Combo > weapon->MaxCombo) {
		Combo = 1;
	}

	if (NowState == EPlayerBehaviorState::Dodge) {
		NowState = EPlayerBehaviorState::Attack;
		Combo = 3;
		// Thrust 애니메이션 실행

		Combo = 0;
		isCombo = false;
		NowState = EPlayerBehaviorState::Idle;
	} else if (NowState != EPlayerBehaviorState::Attack) {
		NowState = EPlayerBehaviorState::Attack;
	}

	GEngine->AddOnScreenDebugMessage(0, 1, FColor::Red, FString::Printf(TEXT("Combo : %d"), Combo));
	// 애니메이션 재생 종료까지 대기? -> 애니메이션이 끝나면 다음 입력 없으면 기본 Idle 로 돌아가기
}

void APlayerZagreus::Dodge(const FInputActionValue& inputValue)
{
	if (NowState != EPlayerBehaviorState::Dodge) {
		NowState = EPlayerBehaviorState::Dodge;
	}

	Speed = 1000.0f;
	// 에너미와 안 부딪히도록 처리
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	
	// 애니메이션 시간이 끝나면 충돌 체크와 스피드 원복
	Speed = 700.0f;
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

// 에너미 오버랩 시 공격은 무기에서
void APlayerZagreus::SpecialAtt(const FInputActionValue& inputValue)
{
	if (NowState != EPlayerBehaviorState::SpecialAtt) {
		NowState = EPlayerBehaviorState::SpecialAtt;
	}
	SetAttackDir();

	// 특수공격 애니메이션 -> 끝나면 NowState 변경
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
