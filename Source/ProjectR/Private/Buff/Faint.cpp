// Fill out your copyright notice in the Description page of Project Settings.

#include "Buff/Faint.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "GameFramework/PlayerController.h"
#include "Character/ProjectRCharacter.h"

void UFaint::Initialize()
{
	Super::Initialize();

	Controller = GetTarget()->GetController();
	bIsPlayer = Controller->IsPlayerController();
}

void UFaint::BeginBuff()
{
	if (++Count > 1)
		return;

	if (bIsPlayer)
		Cast<APlayerController>(Controller)->DisableInput(nullptr);
	else
		if (auto* Brain = Cast<AAIController>(Controller)->GetBrainComponent())
			Brain->StopLogic(TEXT("Faint"));
}

void UFaint::EndBuff()
{
	if (--Count > 0)
		return;

	if (bIsPlayer)
		Cast<APlayerController>(Controller)->EnableInput(nullptr);
	else
		if (auto* Brain = Cast<AAIController>(Controller)->GetBrainComponent())
			Brain->RestartLogic();
}

bool UFaint::IsActivate_Implementation() const
{
	return Count > 0;
}
