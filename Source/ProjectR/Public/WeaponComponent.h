// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTR_API UWeaponComponent final : public UActorComponent
{
	GENERATED_BODY()

public:	
	UWeaponComponent();

	UFUNCTION(BlueprintCallable)
	void StartSkill(uint8 Index);

	UFUNCTION(BlueprintCallable)
	void EndSkill(uint8 Index);

	UFUNCTION(BlueprintCallable)
	bool CanUseSkill(uint8 Index) const;

	UFUNCTION(BlueprintCallable)
	void SwapWeapon(uint8 Index);

	UFUNCTION(BlueprintCallable)
	void CreateNewWeapon(FName Name, uint8 Index);

	void SetWeaponCollision(bool bEnableRight, bool bEnableLeft);

	FORCEINLINE class UWeapon* GetWeapon() noexcept { return Weapons[CurIndex]; }
	FORCEINLINE const UWeapon* GetWeapon() const noexcept { return Weapons[CurIndex]; }

	FORCEINLINE uint8 GetWeaponNum() const noexcept { return WeaponNum; }
	FORCEINLINE uint8 GetWeaponIndex() const noexcept { return CurIndex; }

private:
	void BeginPlay() override;
	void EndPlay(EEndPlayReason::Type EndPlayReason);

	UStaticMeshComponent* CreateWeaponComponent(FName Name);
	void EquipWeapon(UWeapon* NewWeapon, bool bNeedUnequip);

	UFUNCTION()
	void OnWeaponOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void SetWeaponMesh();

	UFUNCTION()
	void Detach(AController* Instigator);

	void DetachOnce(class UStaticMeshComponent* Weapon);

private:
	UPROPERTY()
	UStaticMeshComponent* RightWeapon;

	UPROPERTY()
	UStaticMeshComponent* LeftWeapon;

	UPROPERTY()
	TArray<UWeapon*> Weapons;

	UPROPERTY()
	class AProjectRCharacter* User;

	uint8 WeaponNum;
	uint8 CurIndex;
};
