// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/ProjectRCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"
#include "Buff/Cast.h"
#include "Buff/Lock.h"
#include "Buff/Root.h"
#include "Buff/Run.h"
#include "Character/ProjectRPlayerState.h"
#include "Parryable.h"
#include "WeaponComponent.h"

AProjectRCharacter::AProjectRCharacter()
	: Super()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Hitable"));

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.1f;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 270.0f, 0.0f);
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bAllowPhysicsRotationDuringAnimRootMotion = true;

	WeaponComponent = CreateDefaultSubobject<UWeaponComponent>(TEXT("Weapon"));

	Parrying = nullptr;
	bIsTurning = false;
	bIsDeath = false;
}

void AProjectRCharacter::Attack(AProjectRCharacter* Target, int32 Damage)
{
	auto TakingDamage = static_cast<int32>(Target->
		TakeDamage(Damage, FDamageEvent{}, GetController(), this));

	if (TakingDamage > 0u)
		OnAttack.Broadcast(Target, TakingDamage);
}

void AProjectRCharacter::BeginPlay()
{
	Super::BeginPlay();

	OnAttack.AddDynamic(this, &AProjectRCharacter::HealHealthAndEnergy);
}

float AProjectRCharacter::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	auto Damage = static_cast<int32>(DamageAmount);
	auto* Character = Cast<AProjectRCharacter>(DamageCauser);

	if (UBuffLibrary::GetBuff<UParrying>(this)->ParryIfCan(Damage, EventInstigator, Character))
		return 0.0f;

	Damage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	auto* MyPlayerState = GetPlayerState<AProjectRPlayerState>();
	MyPlayerState->HealHealth(-Damage);

	if (MyPlayerState->GetHealth() == 0u) Death();
	else OnDamaged.Broadcast(EventInstigator, Damage);

	return Damage;
}

void AProjectRCharacter::Landed(const FHitResult& Hit)
{
	OnLand.Broadcast(Hit);
}

void AProjectRCharacter::HealHealthAndEnergy(AProjectRCharacter* Target, int32 Damage)
{
	auto* MyPlayerState = GetPlayerState<AProjectRPlayerState>();
	MyPlayerState->HealHealthByDamage(Damage);
	MyPlayerState->HealEnergyByDamage(Damage);
}

FVector AProjectRCharacter::GetViewLocation_Implementation() const
{
	FVector Vec;
	FRotator Rot;
	GetActorEyesViewPoint(Vec, Rot);
	return Vec;
}

void AProjectRCharacter::Death()
{
	bIsDeath = true;
	OnDeath.Broadcast(LastHitBy);

	// Enable Ragdoll.
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetSimulatePhysics(true);
}
