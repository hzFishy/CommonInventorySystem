// By hzFishy - 2025 - Do whatever you want with it.


#include "Data/Definitions/CISInventoryPresetDefinition.h"
#include "Core/CISInventorySlot.h"


UCISInventoryPresetDefinition::UCISInventoryPresetDefinition():
	SlotClass(UCISInventorySlot::StaticClass())
{}

TSoftClassPtr<UCISInventorySlot> UCISInventoryPresetDefinition::GetInventorySlotClass() const
{
	return SlotClass;
}

void UCISInventoryPresetDefinition::GetInventorySlotDefinitions(TArray<FCISInventorySlotDefinition>& OutSlotDefinitions) const
{
	OutSlotDefinitions.Append(SlotDefinitions);
}

void UCISInventoryPresetDefinition::GetSoftInventoryItems(TArray<TSoftObjectPtr<UCISInventoryItemDefinition>>& ItemDefinitions) const
{
	for (auto& SlotDef : SlotDefinitions)
	{
		ItemDefinitions.Emplace(SlotDef.SoftItemDefinition);
	}
}
