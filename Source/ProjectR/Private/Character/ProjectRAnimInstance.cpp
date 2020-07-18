// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/ProjectRAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Buff/Stun.h"
#include "Buff/Lock.h"
#include "Character/ProjectRCharacter.h"
#include "BuffLibrary.h"

UProjectRAnimInstance::UProjectRAnimInstance()
	: Super()
{
	Speed = 0.0f;
	bIsLooking = false;
	bIsStuned = false;
	bIsInAir = false;
}

void UProjectRAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	const auto* User = Cast<AProjectRCharacter>(TryGetPawnOwner());
	if (!User) return;

	const auto* Movement = User->GetCharacterMovement();
	Velocity = User->GetActorRotation().UnrotateVector(Movement->Velocity);
	Speed = Velocity.Size();
	
	bIsLooking = UBuffLibrary::IsActivate<ULock>(User);
	bIsStuned = UBuffLibrary::IsActivate<UStun>(User);
	bIsInAir = Movement->IsFalling();
}
