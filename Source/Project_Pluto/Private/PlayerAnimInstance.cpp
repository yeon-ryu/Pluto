// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerZagreus.h"
#include "PlayerWeapon.h"

UPlayerAnimInstance::UPlayerAnimInstance()
{
	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerZagreus::StaticClass());
	player = Cast<APlayerZagreus>(actor);
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if(player == nullptr) {
		player = Cast<APlayerZagreus>(TryGetPawnOwner());
		if(player == nullptr) return;
	}

	FVector velocity = player->GetVelocity();
	FVector forwardVector = player->GetActorForwardVector();

	// 앞 뒤 이동속도
	Speed = FVector::DotProduct(forwardVector, velocity);

	// 좌 우 이동속도
	FVector rightVector = player->GetActorRightVector();
	Direction = FVector::DotProduct(rightVector, velocity);
}

void UPlayerAnimInstance::AnimNotify_AttackEffect()
{
	// 플레이어가 가지고 있는 무기의 공격 범위 ovelap 체크 시작
	// 이 순간 부터 우선 순위 최상 -> 회피, 이동, 공격 등 이 애니메이션이 끝나기 전까지 아무것도 적용할 수 없다.
	player->bSpecialAtt = true;
	player->bForceSpecialAtt = true;
	player->SetAttackDir();
	player->weapon->StartSpecialAtt();
}

void UPlayerAnimInstance::AnimNotify_AttackEnd()
{
	// 플레이어가 가지고 있는 무기의 공격 범위 overlap 체크 종료
	player->weapon->EndSpecialAtt();
}

void UPlayerAnimInstance::AnimNotify_SpecialAttEnd()
{
	// 끝나면 NowState 무조건 Idle 로 변경
	player->bSpecialAtt = false;
	player->bForceSpecialAtt = false;
	player->Speed = player->RunSpeed;
	player->NowState = EPlayerBehaviorState::Idle;
}
