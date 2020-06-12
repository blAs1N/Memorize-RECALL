// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectRAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ProjectRCharacter.h"
#include "Weapon.h"

UProjectRAnimInstance::UProjectRAnimInstance()
{
	Speed = 0.0f;
	bIsInAir = false;
}

void UProjectRAnimInstance::NativeBeginPlay()
{
	AProjectRCharacter* Owner = Cast<AProjectRCharacter>(TryGetPawnOwner());
	Owner->OnEquipped.AddDynamic(this, &UProjectRAnimInstance::OnEquipped);
	Owner->BroadcastOnAnimInstanceSpawned(this);

	AWeapon* CurWeapon = GetWeapon();
	if (CurWeapon) OnEquipped(CurWeapon);
}

void UProjectRAnimInstance::OnEquipped(AWeapon* Weapon)
{
	LocomotionSpace = Weapon->GetLocomotionSpace();
	JumpStart = Weapon->GetJumpStart();
	JumpLoop = Weapon->GetJumpLoop();
	JumpEnd = Weapon->GetJumpEnd();
}

void UProjectRAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	ACharacter* Owner = Cast<ACharacter>(TryGetPawnOwner());
	if (!IsValid(Owner)) return;

	Speed = Owner->GetVelocity().Size();
	bIsInAir = Owner->GetCharacterMovement()->IsFalling();
}

void UProjectRAnimInstance::AnimNotify_BeginSkill()
{
	UAnimMontage* Montage = GetCurrentActiveMontage();
	AWeapon* Weapon = GetWeapon();

	if (IsValid(Weapon))
		Weapon->BeginSkill(Montage);
}

void UProjectRAnimInstance::AnimNotify_EndSkill()
{
	UAnimMontage* Montage = GetCurrentActiveMontage();
	AWeapon* Weapon = GetWeapon();

	if (IsValid(Weapon))
		Weapon->EndSkill(Montage, false);
}

void UProjectRAnimInstance::AnimNotify_BeginAttack()
{
	AWeapon* Weapon = GetWeapon();
	
	if (IsValid(Weapon))
		Weapon->OnBeginAttack.Broadcast();
}

void UProjectRAnimInstance::AnimNotify_EndAttack()
{
	AWeapon* Weapon = GetWeapon();
	
	if (IsValid(Weapon))
		Weapon->OnEndAttack.Broadcast();
}

void UProjectRAnimInstance::AnimNotify_Shoot()
{
	AWeapon* Weapon = GetWeapon();
	
	if (IsValid(Weapon))
		Weapon->OnShoot.Broadcast();
}

void UProjectRAnimInstance::AnimNotify_Execute()
{
	AWeapon* Weapon = GetWeapon();
	
	if (IsValid(Weapon))
		Weapon->OnExecute.Broadcast();
}

AWeapon* UProjectRAnimInstance::GetWeapon() const
{
	AProjectRCharacter* Character = Cast<AProjectRCharacter>(TryGetPawnOwner());
	return IsValid(Character) ? Character->GetWeapon() : nullptr;
}