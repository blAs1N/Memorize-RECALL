// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ProjectRCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDamaged, AController*, Instigator);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeath, AController*, Instigator);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEquipped, class AWeapon*, Weapon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAttack, AProjectRCharacter*, Target, int32, Damage);

UCLASS(config=Game, Abstract, Blueprintable)
class AProjectRCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AProjectRCharacter();

	UFUNCTION(BlueprintCallable)
	void Attack(AProjectRCharacter* Target, int32 Damage, AActor* Causer);

	UFUNCTION(BlueprintCallable)
	void BeginParrying(UObject* InParrying);

	UFUNCTION(BlueprintCallable)
	void EndParrying(UObject* InParrying);

	UFUNCTION(BlueprintCallable)
	void ApplyStun();

	UFUNCTION(BlueprintCallable)
	void ReleaseStun();

	UFUNCTION(BlueprintCallable)
	int32 HealHealth(int32 Value);

	UFUNCTION(BlueprintCallable)
	int32 SetMaxHealth(int32 NewMaxHealth);

	UFUNCTION(BlueprintCallable)
	float GetSpeed() const noexcept;

	UFUNCTION(BlueprintCallable)
	void SetSpeed(float Speed) noexcept;

	UFUNCTION(BlueprintNativeEvent)
	void SetLockOn(bool bIsLockOn);

	void SetCastData(bool bCastData = false, bool bMoveData = true) noexcept;

	FORCEINLINE class UStaticMeshComponent* GetLeftWeapon() const noexcept { return LeftWeapon; }
	FORCEINLINE UStaticMeshComponent* GetRightWeapon() const noexcept { return RightWeapon; }

	FORCEINLINE class AWeapon* GetWeapon() const noexcept { return Weapon; }
	FORCEINLINE int32 GetHealth() const noexcept { return Health; }
	FORCEINLINE int32 GetMaxHealth() const noexcept { return MaxHealth; }
	FORCEINLINE float GetHealthHeal() const noexcept { return HealthHeal; }
	FORCEINLINE TMap<TSubclassOf<class ABuff>, class UBuffStorage*>&
		GetBuffStorages() noexcept { return BuffStorages; }

	FORCEINLINE bool IsDeath() const noexcept { return bIsDeath; }

protected:
	void BeginPlay() override;

	float TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable)
	class AWeapon* GenerateWeapon(FName Name);

	UFUNCTION(BlueprintCallable)
	void SetWeapon(AWeapon* InWeapon);

	UFUNCTION(BlueprintImplementableEvent)
	TArray<FName> GetWeaponNames();

	UFUNCTION(BlueprintImplementableEvent)
	void OnStunApply();

	UFUNCTION(BlueprintImplementableEvent)
	void OnStunRelease();

	virtual void NativeOnStunApply() {}
	virtual void NativeOnStunRelease() {}

	void UseSkill(uint8 index);

	FORCEINLINE bool IsCasting() const noexcept { return bIsCasting; }
	FORCEINLINE bool CanMoving() const noexcept { return bCanMoving; }

private:
	virtual void CreateWeapons(TArray<FName>&& WeaponNames);

	void Death();

	UFUNCTION()
	void OnAttacked(AProjectRCharacter* Target, int32 Damage);

	UFUNCTION()
	void Equip();

	FORCEINLINE void SetLockOn_Implementation(bool bIsLockOn) {}

public:
	UPROPERTY(BlueprintAssignable)
	FOnDamaged OnDamaged;

	UPROPERTY(BlueprintAssignable)
	FOnDeath OnDeath;

	UPROPERTY(BlueprintAssignable)
	FOnEquipped OnEquipped;

	UPROPERTY(BlueprintAssignable)
	FOnAttack OnAttack;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* LeftWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* RightWeapon;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient, Category = Weapon, meta = (AllowPrivateAccess = true))
	AWeapon* Weapon;

	UPROPERTY(EditDefaultsOnly, Category = Weapon, meta = (AllowPrivateAccess = true))
	class UDataTable* WeaponsData;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Transient, Category = Stat, meta = (AllowPrivateAccess = true))
	int32 Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Transient, Category = Stat, meta = (AllowPrivateAccess = true))
	int32 MaxHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Transient, Category = Stat, meta = (AllowPrivateAccess = true))
	float HealthHeal;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Buff, meta = (AllowPrivateAccess = true))
	TMap<TSubclassOf<class ABuff>, class UBuffStorage*> BuffStorages;

	UObject* Parrying;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient, meta = (AllowPrivateAccess = true))
	uint8 bIsDeath : 1;

	uint8 bIsCasting : 1;
	uint8 bCanMoving : 1;
};