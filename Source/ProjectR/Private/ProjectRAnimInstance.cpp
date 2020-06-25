// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectRAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "ProjectRCharacter.h"
#include "Weapon.h"
#include "WeaponComponent.h"

UProjectRAnimInstance::UProjectRAnimInstance()
	: Super()
{
	User = nullptr;
	Speed = 0.0f;
	bIsLooking = false;
	bIsInAir = false;
}

void UProjectRAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	OnMontageStarted.AddDynamic(this, &UProjectRAnimInstance::OnBeginMontage);
	OnMontageEnded.AddDynamic(this, &UProjectRAnimInstance::OnEndMontage);

	User = Cast<AProjectRCharacter>(TryGetPawnOwner());
}

void UProjectRAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!IsValid(User)) return;

	const auto* Movement = User->GetCharacterMovement();
	Velocity = User->GetActorRotation().UnrotateVector(Movement->Velocity);
	Speed = Velocity.Size();
	
	bIsLooking = User->IsLooking();
	bIsInAir = Movement->IsFalling();
}

void UProjectRAnimInstance::OnBeginMontage(UAnimMontage* Montage)
{
	AnimNotify_BeginSkill();
}

void UProjectRAnimInstance::OnEndMontage(UAnimMontage* Montage, bool bInterrupted)
{
	AnimNotify_EndSkill();
}

void UProjectRAnimInstance::AnimNotify_BeginSkill()
{
	User->SetCast(true);
	GetWeapon()->OnBeginSkill.Broadcast();
}

void UProjectRAnimInstance::AnimNotify_EndSkill()
{
	User->SetCast(false);
	GetWeapon()->OnEndSkill.Broadcast();
}

void UProjectRAnimInstance::AnimNotify_BeginAttack()
{
	GetWeapon()->OnBeginAttack.Broadcast();
}

void UProjectRAnimInstance::AnimNotify_EndAttack()
{
	GetWeapon()->OnEndAttack.Broadcast();
}

void UProjectRAnimInstance::AnimNotify_Shoot()
{
	GetWeapon()->OnShoot.Broadcast();
}

void UProjectRAnimInstance::AnimNotify_Execute()
{
	GetWeapon()->OnExecute.Broadcast();
}

void UProjectRAnimInstance::AnimNotify_EnableMove()
{
	User->SetMove(true);
}

void UProjectRAnimInstance::AnimNotify_DisableMove()
{
	User->SetMove(false);
}

UWeapon* UProjectRAnimInstance::GetWeapon() const
{
	return User->GetWeaponComponent()->GetWeapon();
}
