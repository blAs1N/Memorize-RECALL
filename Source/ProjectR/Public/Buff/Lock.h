// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Buff/Buff.h"
#include "Lock.generated.h"

UCLASS(BlueprintType)
class PROJECTR_API ULock final : public UBuff
{
	GENERATED_BODY()

public:
	FORCEINLINE void SetLockedTarget(class AProjectRCharacter*
		InLockedTarget) noexcept { LockedTarget = InLockedTarget; }
	
	FORCEINLINE AProjectRCharacter* GetLockedTarget()
		const noexcept { return LockedTarget; }

private:
	void Tick(float DeltaSeconds) override;

	void OnApply() override;
	void OnRelease() override;

	bool IsActivate_Implementation() const override;

private:
	UPROPERTY()
	AProjectRCharacter* LockedTarget;

	uint8 bIsLocked : 1;
};