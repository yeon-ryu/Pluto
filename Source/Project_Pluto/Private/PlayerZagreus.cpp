// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerZagreus.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"

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
		UE_LOG(LogTemp, Warning, TEXT("SkeletalMesh loding fail"));
	}

	// 초기 카메라 설정
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	springArmComp->SetupAttachment(RootComponent);
	springArmComp->SetRelativeLocationAndRotation(FVector(-200.0f, 100.0f, 200.0f), FRotator(-50.0f, -30.0f, 0.0f));
	springArmComp->TargetArmLength = 700.0f;
	springArmComp->bDoCollisionTest = false;

	camComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CamComp"));
	camComp->SetupAttachment(springArmComp);
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
}

// Called every frame
void APlayerZagreus::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	{
		// 플레이어 이동
		PlayerDir.Normalize();
		SetActorLocation(GetActorLocation() + PlayerDir * Speed * DeltaTime);
		PlayerDir = FVector::ZeroVector;
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

void APlayerZagreus::SetAttackDir()
{
	auto PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController) {
		float MouseX, MouseY;
		PlayerController->GetMousePosition(MouseX, MouseY);

		MouseLocation.X = MouseX;
		MouseLocation.Y = MouseY;

		AttackDirection = MouseLocation - GetActorLocation();
	}
}

void APlayerZagreus::Move(const FInputActionValue& inputValue)
{
	FVector2D value = inputValue.Get<FVector2D>();
	PlayerDir.X = value.X;
	PlayerDir.Y = value.Y;
}

void APlayerZagreus::Attack(const FInputActionValue& inputValue)
{
	GEngine->AddOnScreenDebugMessage(0, 1, FColor::Red, TEXT("Attack"));
	SetAttackDir();
	UE_LOG(LogTemp, Warning, TEXT("Player : %.1f , %.1f"), GetActorLocation().X, GetActorLocation().Y);
	UE_LOG(LogTemp, Warning, TEXT("Mouse : %.1f , %.1f"), MouseLocation.X, MouseLocation.Y);
}

void APlayerZagreus::Dodge(const FInputActionValue& inputValue)
{
}

void APlayerZagreus::SpecialAtt(const FInputActionValue& inputValue)
{
	SetAttackDir();
}

void APlayerZagreus::Spell(const FInputActionValue& inputValue)
{
	SetAttackDir();
}

void APlayerZagreus::Interaction(const FInputActionValue& inputValue)
{
}
