// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "WeaponData.generated.h"

USTRUCT(Atomic, BlueprintType)
struct FWeaponData : public FTableRowBase
{
	GENERATED_BODY()

	FWeaponData();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Key;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TAssetPtr<class UStaticMesh> RightMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform RightTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TAssetPtr<UStaticMesh> LeftMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform LefttTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, EditFixedSize)
	TArray<TSubclassOf<class ASkill>> Skills;
};