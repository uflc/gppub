// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/GPItem.h"
#include "GPWeaponActorBase.h"
#include "GPItemWeapon.generated.h"

/**
 * 
 */
UCLASS()
class GPROJECT_API UGPItemWeapon : public UGPItem
{
	GENERATED_BODY()

public:
	UGPItemWeapon()
	{
		ItemType = UGPAssetManager::WeaponItemType;
	}

	/** Weapon actor to spawn */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	TSubclassOf<AGPWeaponActorBase> WeaponActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	FSlateBrush WeaponVerticalIcon;
};
