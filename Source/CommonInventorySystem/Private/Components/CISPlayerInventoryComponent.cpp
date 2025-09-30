// By hzFishy - 2025 - Do whatever you want with it.


#include "Components/CISPlayerInventoryComponent.h"
#include "Asserts/FUAsserts.h"
#include "Data/CISInventoryDeveloperSettings.h"
#include "Data/Definitions/CISPlayerInventoryPresetDefinition.h"
#include "Console/FUConsole.h"


#if FU_WITH_CONSOLE
namespace CIS::Core::Debug
{
	FU_CMD_OBJECT_ALLRUNFUNC(DumpAllSlotsForPlayer,
		"CIS.Inventory.DumpAllSlotsForPlayer", "Dump info for all slots for unique player inventory components",
		UCISPlayerInventoryComponent, DumpAllSlots);
}
#endif

	
	/*----------------------------------------------------------------------------
		Defaults
	----------------------------------------------------------------------------*/
UCISPlayerInventoryComponent::UCISPlayerInventoryComponent()
{
	
}

	
	/*----------------------------------------------------------------------------
		Init
	----------------------------------------------------------------------------*/
void UCISPlayerInventoryComponent::OnInventoryItemDefinitionsLoaded(TSoftClassPtr<UCISInventorySlot> SoftInventorySlotClass,
	TArray<FCISInventorySlotDefinition> SlotDefinitions)
{
	Super::OnInventoryItemDefinitionsLoaded(SoftInventorySlotClass, SlotDefinitions);

	auto* PresetDefinitionObject = InventoryPresetDefinition.Get();
	if (!FU_ENSURE_VALID(PresetDefinitionObject)) { return; }
	
	// init hotbar slots
	if (auto* PlayerDefinition = Cast<UCISPlayerInventoryPresetDefinition>(PresetDefinitionObject))
	{
		auto HotbarInventoryTag = InventoryDeveloperSettings->HotbarInventoryCategoryTag;
		InventorySlots.Emplace(HotbarInventoryTag, FCISInventorySlotCategory(PlayerDefinition->HotbarSize));
		for (auto i = 0; i < PlayerDefinition->HotbarSize; i++)
		{
			auto& NewSlot = InventorySlots[HotbarInventoryTag].Slots.Emplace_GetRef(CreateSlot(HotbarInventoryTag, i));
			OnInventorySlotAddedDelegate.Broadcast(HotbarInventoryTag, NewSlot.Get());
		}
	}
}
