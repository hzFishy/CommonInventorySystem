// By hzFishy - 2025 - Do whatever you want with it.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "CISInventoryDefinitionInterface.generated.h"
class UCISInventoryItemDefinition;
class UCISInventorySlot;


USTRUCT()
struct COMMONINVENTORYSYSTEM_API FCISInventorySlotDefinition
{
	GENERATED_BODY()

	FCISInventorySlotDefinition();

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UCISInventoryItemDefinition> SoftItemDefinition;
	
	UPROPERTY(EditAnywhere)
	int32 Count;
};

USTRUCT()
struct COMMONINVENTORYSYSTEM_API FCISInventoryCategoryDefinition
{
	GENERATED_BODY()

	FCISInventoryCategoryDefinition();

	/** The class of the slots for this category */
	UPROPERTY(EditAnywhere, Category="CommonInventorySystem")
	TSoftClassPtr<UCISInventorySlot> SlotClass;
	
	UPROPERTY(EditAnywhere)
	TArray<FCISInventorySlotDefinition> SlotDefinitions;
};


UINTERFACE()
class UCISInventoryDefinitionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class COMMONINVENTORYSYSTEM_API ICISInventoryDefinitionInterface
{
	GENERATED_BODY()

public:
	virtual void GetInventorySlotCategories(TArray<FGameplayTag>& Categories) const;
	
	virtual void GetInventorySlotCategoryDefinition(FGameplayTag Category, FCISInventoryCategoryDefinition& OutDefinition) const;
	
	virtual TSoftClassPtr<UCISInventorySlot> GetInventorySlotClass(FGameplayTag Category) const;

	virtual void GetInventorySlotDefinitions(FGameplayTag Category, TArray<FCISInventorySlotDefinition>& SlotDefinitions) const;
	
	virtual void GetSoftInventoryItems(FGameplayTag Category, TArray<TSoftObjectPtr<UCISInventoryItemDefinition>>& ItemDefinitions) const;
};
