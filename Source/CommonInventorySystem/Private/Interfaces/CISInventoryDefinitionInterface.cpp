// By hzFishy - 2025 - Do whatever you want with it.


#include "Interfaces/CISInventoryDefinitionInterface.h"


FCISInventorySlotDefinition::FCISInventorySlotDefinition():
	Count(1)
{}

TSoftClassPtr<UCISInventorySlot> ICISInventoryDefinitionInterface::GetInventorySlotClass() const
{
	unimplemented();
	return nullptr;
}

void ICISInventoryDefinitionInterface::GetInventorySlotDefinitions(TArray<FCISInventorySlotDefinition>& SlotDefinitions) const
{
	unimplemented();
}

void ICISInventoryDefinitionInterface::GetSoftInventoryItems(TArray<TSoftObjectPtr<UCISInventoryItemDefinition>>& ItemDefinitions) const
{
	unimplemented();
}
