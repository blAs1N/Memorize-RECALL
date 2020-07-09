// Fill out your copyright notice in the Description page of Project Settings.

#include "Buff/Root.h"
#include "Buff/Lock.h"
#include "Character/ProjectRCharacter.h"
#include "BuffLibrary.h"

void URoot::OnApply()
{
	bIsRoot = true;
	UBuffLibrary::BlockBuff<ULock>(GetTarget());
}

void URoot::OnRelease()
{
	bIsRoot = false;
	UBuffLibrary::UnblockBuff<ULock>(GetTarget());
}

bool URoot::IsActivate_Implementation() const
{
	return bIsRoot;
}