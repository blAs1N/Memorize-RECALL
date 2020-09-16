// Fill out your copyright notice in the Description page of Project Settings.

#include "Skill/Leap.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Data/LeapData.h"
#include "Framework/PRCharacter.h"
#include "Misc/Spline.h"

ULeap::ULeap()
	: Super()
{
	Spline = CreateDefaultSubobject<USpline>(TEXT("Spline"));
	Spline->GetPoints().Init(FVector::ZeroVector, 5);
}

void ULeap::Begin(USkillContext* InContext, const UDataAsset* Data)
{
	Super::Begin(InContext, Data);

	SetData(Data);
	InitSpline();

	GetUser()->GetCharacterMovement()
		->SetMovementMode(EMovementMode::MOVE_Flying);
}

void ULeap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsLeapEnded) return;

	if (Idx == 1)
		UpdateSpline();
	
	UpdateLeap(DeltaTime);

	FVector NewLocation;
	if (Spline->Compute(Idx, Alpha, NewLocation))
	{
		if (!GetUser()->SetActorLocation(NewLocation, true))
			Finish();
	}
	else Finish();
}

void ULeap::End()
{
	GetUser()->GetCharacterMovement()
		->SetMovementMode(EMovementMode::MOVE_Walking);

	Super::End();
}

void ULeap::SetData(const UDataAsset* Data)
{
	const auto* MyData = Cast<const ULeapData>(Data);
	MaxHeight = MyData->MaxHeight;
	LeapDuration = MyData->LeapDuration;
	AttackRange = MyData->AttackRange;
	MaxHeightRatio = MyData->MaxHeightRatio;
	LeapTimeRatio = MyData->LeapTimeRatio;
}

void ULeap::InitSpline()
{
	StartLocation = GetUser()->GetActorLocation();
	Target = GetUser()->GetLockedTarget();

	FVector RangeVec = Target->GetActorLocation() - StartLocation;
	RangeVec.Z = 0.0f;
	RangeVec.Normalize();
	RangeVec *= AttackRange;

	Spline->GetPoints()[0] = StartLocation - RangeVec;
	Spline->GetPoints()[1] = StartLocation;

	Idx = 1;
	Alpha = 0.0f;
	bIsLeapEnded = false;
}

void ULeap::UpdateSpline()
{
	FVector TargetLocation = Target->GetActorLocation();
	TargetLocation.Z = StartLocation.Z;

	FVector RangeVec = TargetLocation - StartLocation;
	RangeVec.Normalize();
	RangeVec *= AttackRange;

	auto& Point2 = Spline->GetPoints()[2];
	Point2 = FMath::Lerp(StartLocation, TargetLocation, MaxHeightRatio);
	Point2.Z += MaxHeight;

	Spline->GetPoints()[3] = TargetLocation - RangeVec;
	Spline->GetPoints()[4] = TargetLocation;
}

void ULeap::UpdateLeap(float DeltaTime)
{
	float TimeRatio = Idx == 1 ? LeapTimeRatio : 1 - LeapTimeRatio;
	TimeRatio *= LeapDuration;

	Alpha += DeltaTime / TimeRatio;
	if (Alpha <= 1.0f) return;

	if (Idx == 1)
	{
		Idx = 2;
		Alpha = FMath::Fmod(Alpha, 1.0f);
	}
	else
	{
		Alpha = 1.0f;
		bIsLeapEnded = true;
	}
}