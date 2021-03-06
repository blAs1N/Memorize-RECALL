// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Data/VisualData.h"
#include "Weapon.generated.h"

DECLARE_DELEGATE(FOnAsyncLoadEndedSingle);
DECLARE_MULTICAST_DELEGATE(FOnAsyncLoadEnded);

USTRUCT()
struct FUsableSkill
{
	GENERATED_BODY()

	UPROPERTY()
	class USkill* Skill;

	UPROPERTY()
	class UDataAsset* Data;

	uint8 bIsOverrided : 1;
};

UCLASS(BlueprintType)
class PROJECTR_API UWeapon final : public UObject
{
	GENERATED_BODY()
	
public:
	UWeapon();
	bool Initialize(class USkillContext* InContext, uint8 InKey);
	void InitSkill(uint8 Level);

	void BeginSkill(uint8 Index);
	void EndSkill(uint8 Index);

	void TickSkill(uint8 Index, float DeltaTime);
	
	void RegisterOnAsyncLoadEnded(const FOnAsyncLoadEndedSingle& Callback);

	void Execute(uint8 Index);
	void BeginExecute(uint8 Index);
	void EndExecute(uint8 Index);

	FORCEINLINE const FVisualData& GetVisualData() const noexcept { return VisualData; }
	FORCEINLINE uint8 GetKey() const noexcept { return Key; }

private:
	void LoadAll(const struct FWeaponData& WeaponData);

private:
	UPROPERTY(Transient)
	class APRCharacter* User;
	
	UPROPERTY(Transient)
	TArray<FUsableSkill> Skills;

	UPROPERTY(Transient)
	USkillContext* Context;

	UPROPERTY(Transient)
	FVisualData VisualData;

	UPROPERTY()
	class UDataTable* WeaponDataTable;

	UPROPERTY()
	UDataTable* SkillDataTable;

	FOnAsyncLoadEnded OnAsyncLoadEnded;

	uint8 Key;
	uint8 AsyncLoadCount;
};
