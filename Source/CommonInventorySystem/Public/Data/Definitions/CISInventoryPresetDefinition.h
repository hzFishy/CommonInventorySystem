// By hzFishy - 2025 - Do whatever you want with it.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "Interfaces/CISInventoryDefinitionInterface.h"
#include "CISInventoryPresetDefinition.generated.h"
class UCISInventoryItemDefinition;
class UCISInventorySlot;


/**
 * Defines the inventory attributs (size, default items, ...)
 */
UCLASS(ClassGroup=CommonInventorySystem, DisplayName="Inventory Preset Definition")
class COMMONINVENTORYSYSTEM_API UCISInventoryPresetDefinition : public UPrimaryDataAsset, public ICISInventoryDefinitionInterface
{
	GENERATED_BODY()

public:
	UCISInventoryPresetDefinition();
	
	/**
	 * Default items, the inventory slots size will match with the size of this array.
	 * Per Category
	 */
	UPROPERTY(EditAnywhere, Category="CommonInventorySystem", meta=(TitleProperty="{SoftItemDefinition} x{Count}", ForceInlineRow))
	TMap<FGameplayTag, FCISInventoryCategoryDefinition> SlotDefinitions;


	virtual void GetInventorySlotCategories(TArray<FGameplayTag>& Categories) const override;

	virtual void GetInventorySlotCategoryDefinition(FGameplayTag Category, FCISInventoryCategoryDefinition& OutDefinition) const override;
	
	virtual TSoftClassPtr<UCISInventorySlot> GetInventorySlotClass(FGameplayTag Category) const override;

	virtual void GetInventorySlotDefinitions(FGameplayTag Category, TArray<FCISInventorySlotDefinition>& OutSlotDefinitions) const override;
	
	virtual void GetSoftInventoryItems(FGameplayTag Category, TArray<TSoftObjectPtr<UCISInventoryItemDefinition>>& ItemDefinitions) const override;
};
