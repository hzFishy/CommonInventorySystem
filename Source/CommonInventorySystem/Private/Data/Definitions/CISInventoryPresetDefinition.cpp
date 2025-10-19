// By hzFishy - 2025 - Do whatever you want with it.


#include "Data/Definitions/CISInventoryPresetDefinition.h"


UCISInventoryPresetDefinition::UCISInventoryPresetDefinition() {}

void UCISInventoryPresetDefinition::GetInventorySlotCategories(TArray<FGameplayTag>& Categories) const
{
	SlotDefinitions.GetKeys(Categories);
}

void UCISInventoryPresetDefinition::GetInventorySlotCategoryDefinition(FGameplayTag Category, FCISInventoryCategoryDefinition& OutDefinition) const
{
	OutDefinition = SlotDefinitions[Category];
}

TSoftClassPtr<UCISInventorySlot> UCISInventoryPresetDefinition::GetInventorySlotClass(FGameplayTag Category) const
{
	if (!SlotDefinitions.Contains(Category)) { return nullptr; }
	
	return SlotDefinitions[Category].SlotClass;
}

void UCISInventoryPresetDefinition::GetInventorySlotDefinitions(FGameplayTag Category, TArray<FCISInventorySlotDefinition>& OutSlotDefinitions) const
{
	if (!SlotDefinitions.Contains(Category)) { return; }
	
	OutSlotDefinitions.Append(SlotDefinitions[Category].SlotDefinitions);
}

void UCISInventoryPresetDefinition::GetSoftInventoryItems(FGameplayTag Category, TArray<TSoftObjectPtr<UCISInventoryItemDefinition>>& ItemDefinitions) const
{
	if (!SlotDefinitions.Contains(Category)) { return; }
	
	for (auto& SlotDef : SlotDefinitions[Category].SlotDefinitions)
	{
		ItemDefinitions.Emplace(SlotDef.SoftItemDefinition);
	}
}
