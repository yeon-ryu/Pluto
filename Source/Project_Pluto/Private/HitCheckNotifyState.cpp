// Fill out your copyright notice in the Description page of Project Settings.


#include "HitCheckNotifyState.h"
#include "EnemyInfo.h"
#include "Components/CapsuleComponent.h"
#include "PlayerZagreus.h"
#include "Kismet/GameplayStatics.h"


void UHitCheckNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	Owner = MeshComp->GetOwner();

	OwnCharacter = Cast<AEnemyInfo>(Owner);
	

	if (OwnCharacter)
	{
		CapsuleComp = Cast<UCapsuleComponent>(OwnCharacter->GetCapsuleComponent());
		if (CapsuleComp)
		{
			// 충돌 이벤트 등록
			CapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &UHitCheckNotifyState::OnPlayerOverlap);
		}

	}
}


void UHitCheckNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (CapsuleComp)
	{
		// 충돌 이벤트 삭제
		CapsuleComp->OnComponentBeginOverlap.RemoveDynamic(this, &UHitCheckNotifyState::OnPlayerOverlap);
	}
}

void UHitCheckNotifyState::OnPlayerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	APlayerZagreus* player = Cast<APlayerZagreus>(OtherActor);
	if (!!player)
	{
		UGameplayStatics::ApplyDamage(player, OwnCharacter->GetDamage(), OwnCharacter->GetController(), OwnCharacter, UDamageType::StaticClass());

		UE_LOG(LogTemp, Warning, TEXT("%s hit %s! Applied %.2f Damage!"), *OwnCharacter->GetName(), *player->GetName(), OwnCharacter->GetDamage ());

		OwnCharacter->GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECR_Ignore);
	}

}
