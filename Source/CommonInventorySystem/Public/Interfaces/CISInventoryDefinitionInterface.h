// By hzFishy - 2025 - Do whatever you want with it.

#pragma once

#include "CoreMinimal.h"
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
	virtual TSoftClassPtr<UCISInventorySlot> GetInventorySlotClass() const;

	virtual void GetInventorySlotDefinitions(TArray<FCISInventorySlotDefinition>& SlotDefinitions) const;
	
	virtual void GetSoftInventoryItems(TArray<TSoftObjectPtr<UCISInventoryItemDefinition>>& ItemDefinitions) const;
};
