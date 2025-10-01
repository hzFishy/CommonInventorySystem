// By hzFishy - 2025 - Do whatever you want with it.

#pragma once

#include "GameplayTagContainer.h"
#include "Data/Fragments/CISBaseInventoryItemFragment.h"
#include "Engine/DataAsset.h"
#include "StructUtils/InstancedStruct.h"
#include "CISInventoryItemDefinition.generated.h"
class UCISInventoryItem;


/**
 * Defines an Item
 */
UCLASS(ClassGroup=CommonInventorySystem, DisplayName="Inventory Item Definition")
class COMMONINVENTORYSYSTEM_API UCISInventoryItemDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UCISInventoryItemDefinition();

	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

	
	UPROPERTY(EditAnywhere, Category="CommonInteractionSystem")
	TSoftClassPtr<UCISInventoryItem> ItemClass;
	
	UPROPERTY(EditAnywhere, Category="CommonInteractionSystem", meta=(Categories="Inventory.Items"))
	FGameplayTag Tag;
	
	UPROPERTY(EditAnywhere, Category="CommonInteractionSystem", meta=(Categories="Inventory.Traits"))
	FGameplayTagContainer TraitsTags;
	
	UPROPERTY(EditAnywhere, Category="CommonInteractionSystem", meta=(ExcludeBaseStruct))
	TArray<TInstancedStruct<FCISBaseInventoryItemFragment>> Fragments;

	UFUNCTION(BlueprintCallable, Category="CommonInventorySystem", BlueprintPure=false)
	bool GetFragmentFromType(
		UPARAM(meta=(MetaStruct="/Script/CommonInventorySystem.CISBaseInventoryItemFragment")) UScriptStruct* Type,
		bool bAllowChilds,
		TInstancedStruct<FCISBaseInventoryItemFragment>& FoundStruct) const;

	template<std::derived_from<FCISBaseInventoryItemFragment> T>
	const T* GetFragmentFromType()
	{
		for (const auto& Fragment : Fragments)
		{
			if (const T* FragmentPtr = Fragment.GetPtr<T>())
			{
				return FragmentPtr;
			}
		}
		return nullptr;
	}
	
	template<std::derived_from<FCISBaseInventoryItemFragment> T>
	T* GetMutableFragmentFromType()
	{
		for (const auto& Fragment : Fragments)
		{
			if (T* FragmentPtr = Fragment.GetPtr<T>())
			{
				return FragmentPtr;
			}
		}
		return nullptr;
	}
};
