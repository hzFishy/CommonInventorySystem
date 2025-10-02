// By hzFishy - 2025 - Do whatever you want with it.

#pragma once

#include "CoreMinimal.h"
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
	
	
	/** The class of the slots */
	UPROPERTY(EditAnywhere, Category="CommonInventorySystem")
	TSoftClassPtr<UCISInventorySlot> SlotClass;
	
	/** Default items, the inventory slots size will match with the size of this array */
	UPROPERTY(EditAnywhere, Category="CommonInventorySystem", meta=(TitleProperty="{SoftItemDefinition} x{Count}"))
	TArray<FCISInventorySlotDefinition> SlotDefinitions;

	
	virtual TSoftClassPtr<UCISInventorySlot> GetInventorySlotClass() const override;

	virtual void GetInventorySlotDefinitions(TArray<FCISInventorySlotDefinition>& OutSlotDefinitions) const override;
	
	virtual void GetSoftInventoryItems(TArray<TSoftObjectPtr<UCISInventoryItemDefinition>>& ItemDefinitions) const override;
};
