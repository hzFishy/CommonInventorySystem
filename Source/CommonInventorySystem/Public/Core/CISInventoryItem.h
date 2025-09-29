// By hzFishy - 2025 - Do whatever you want with it.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Data/Fragments/CISBaseInventoryItemFragment.h"
#include "StructUtils/InstancedStruct.h"
#include "UObject/Object.h"
#include "CISInventoryItem.generated.h"
class UCISInventoryItemDefinition;


/**
 * A living instance of an item.
 * Outer is the inventory slot which holds it.
 */
UCLASS(ClassGroup=CommonInventorySystem, DisplayName="Inventory Item", BlueprintType, Blueprintable)
class COMMONINVENTORYSYSTEM_API UCISInventoryItem : public UObject
{
	GENERATED_BODY()

	
	/*----------------------------------------------------------------------------
		Properties
	----------------------------------------------------------------------------*/
protected:
	FGameplayTag Tag;
	
	FGameplayTagContainer TraitsTags;;
	
	TArray<TInstancedStruct<FCISBaseInventoryItemFragment>> Fragments;
	
	
	/*----------------------------------------------------------------------------
		Defaults
	----------------------------------------------------------------------------*/
public:
	UCISInventoryItem();

	
	/*----------------------------------------------------------------------------
		Core
	----------------------------------------------------------------------------*/
public:
	void InitFromDefinition(UCISInventoryItemDefinition* Definition);

	UFUNCTION(BlueprintPure, Category="CommonInventorySystem")
	FGameplayTag GetTag() const { return Tag; };

	UFUNCTION(BlueprintPure, Category="CommonInventorySystem")
	const FGameplayTagContainer& GetTraitsTags() const { return TraitsTags; };

	UFUNCTION(BlueprintPure, Category="CommonInventorySystem")
	bool HasTrait(FGameplayTag TraitToCheck) const;

	UFUNCTION(BlueprintPure, Category="CommonInventorySystem")
	bool HasTraits(FGameplayTagContainer TraitsToCheck) const;

	int32 GetFragmentCount() const { return Fragments.Num(); };
	
	FString GetFragmentsDebugString() const;

	UFUNCTION(BlueprintCallable, Category="CommonInventorySystem", BlueprintPure=false)
	bool GetFragmentFromType(
		UPARAM(meta=(MetaStruct="/Script/CommonInventorySystem.CISBaseInventoryItemFragment")) UScriptStruct* Type,
		bool bAllowChilds,
		TInstancedStruct<FCISBaseInventoryItemFragment>& FoundStruct) const;
};
