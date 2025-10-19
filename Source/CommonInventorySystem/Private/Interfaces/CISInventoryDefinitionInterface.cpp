// By hzFishy - 2025 - Do whatever you want with it.


#include "Interfaces/CISInventoryDefinitionInterface.h"
#include "Core/CISInventorySlot.h"


FCISInventorySlotDefinition::FCISInventorySlotDefinition():
	Count(1)
{}

FCISInventoryCategoryDefinition::FCISInventoryCategoryDefinition():
	SlotClass(UCISInventorySlot::StaticClass())
{}


void ICISInventoryDefinitionInterface::GetInventorySlotCategories(TArray<FGameplayTag>& Categories) const
{
	
}

void ICISInventoryDefinitionInterface::GetInventorySlotCategoryDefinition(
	FGameplayTag Category, FCISInventoryCategoryDefinition& OutDefinition) const
{
	unimplemented();
	return;
}

TSoftClassPtr<UCISInventorySlot> ICISInventoryDefinitionInterface::GetInventorySlotClass(FGameplayTag Category) const
{
	unimplemented();
	return nullptr;
}

void ICISInventoryDefinitionInterface::GetInventorySlotDefinitions(FGameplayTag Category, TArray<FCISInventorySlotDefinition>& SlotDefinitions) const
{
	unimplemented();
}

void ICISInventoryDefinitionInterface::GetSoftInventoryItems(FGameplayTag Category, TArray<TSoftObjectPtr<UCISInventoryItemDefinition>>& ItemDefinitions) const
{
	unimplemented();
}
