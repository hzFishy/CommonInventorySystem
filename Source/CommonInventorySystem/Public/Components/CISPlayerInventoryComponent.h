// By hzFishy - 2025 - Do whatever you want with it.

#pragma once

#include "CoreMinimal.h"
#include "CISBaseInventoryComponent.h"
#include "CISPlayerInventoryComponent.generated.h"


UCLASS(ClassGroup=(CommonInventorySystem), DisplayName="Player Inventory Component", meta=(BlueprintSpawnableComponent))
class COMMONINVENTORYSYSTEM_API UCISPlayerInventoryComponent : public UCISBaseInventoryComponent
{
	GENERATED_BODY()

	
	/*----------------------------------------------------------------------------
		Defaults
	----------------------------------------------------------------------------*/
public:
	UCISPlayerInventoryComponent();


	/*----------------------------------------------------------------------------
		Init
	----------------------------------------------------------------------------*/
protected:
	virtual void OnInventoryItemDefinitionsLoaded(TSoftClassPtr<UCISInventorySlot> SoftInventorySlotClass,
		TArray<FCISInventorySlotDefinition> SlotDefinitions) override;
};
