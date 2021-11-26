// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GProject.h"
#include "AbilitySystemComponent.h"
#include "Types/GPAbilityTypes.h"
#include "GPAbilitySystemComponent.generated.h"


class UGPGameplayAbility;

/**
 * 
 */


UCLASS()
class GPROJECT_API UGPAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTagUpdated, const FGameplayTag&, Tag, bool, TagExists);

public:
	// Constructors and overrides
	UGPAbilitySystemComponent();

	/** Returns a list of currently active ability instances that match the tags */
	void GetActiveAbilitiesWithTags(const FGameplayTagContainer& GameplayTagContainer, TArray<UGPGameplayAbility*>& ActiveAbilities);

	UFUNCTION(BlueprintCallable)
	bool GetCooldownRemainingForTag(FGameplayTagContainer CooldownTags, float& TimeRemaining, float& CooldownDuration);

	/** Returns the default level used for ability activations, derived from the character */
	int32 GetDefaultAbilityLevel() const;

	UFUNCTION(BlueprintCallable)
	bool CancelAbilityByClass(TSubclassOf<UGameplayAbility> AbilityClass);

	/** Version of function in AbilitySystemGlobals that returns correct type */
	static UGPAbilitySystemComponent* GetAbilitySystemComponentFromActor(const AActor* Actor, bool LookForComponent = false);

	virtual void OnTagUpdated(const FGameplayTag& Tag, bool TagExists) override;

	//UPROPERTY(BlueprintAssignable)
	FOnTagUpdated OnTagUpdatedDelegate;

	friend UGameplayAbility;
protected:
	//AI�� ���÷��̼� ���� �Լ����� ȣ�� ���ϵ��� �Ǿ��ִ� ��.
	/*virtual void OnRep_ActivateAbilities() override;
	virtual void OnGiveAbility(FGameplayAbilitySpec& AbilitySpec) override;*/
};
