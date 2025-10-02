// By hzFishy - 2025 - Do whatever you want with it.


#include "Components/CISPlayerInventoryComponent.h"

#include "MathUtil.h"
#include "Asserts/FUAsserts.h"
#include "Data/CISInventoryDeveloperSettings.h"
#include "Data/Definitions/CISPlayerInventoryPresetDefinition.h"
#include "Console/FUConsole.h"
#include "Core/CISInventoryItem.h"
#include "Core/CISInventorySlot.h"
#include "Data/Fragments/CISActorInventoryItemFragment.h"


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
UCISPlayerInventoryComponent::UCISPlayerInventoryComponent():
	bInitialSelectedHotbarIndexSlotApplied(false),
	HotbarMaxIndex(-1),
	CurrentSelectedSlotIndex(-1),
	FreezeSelectedSlotCount(false)
{
	bWantsInitializeComponent = true;
}

void UCISPlayerInventoryComponent::InitializeComponent()
{
	Super::InitializeComponent();

	if (InitialSelectedHotbarIndexSlot.IsSet())
	{
		UpdateSelectedHotbarSlot(CurrentSelectedSlotIndex + 2 + InitialSelectedHotbarIndexSlot.GetValue());
		bInitialSelectedHotbarIndexSlotApplied = true;
	}
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
		HotbarMaxIndex = PlayerDefinition->HotbarSize - 1;
		for (auto i = 0; i < PlayerDefinition->HotbarSize; i++)
		{
			auto& NewSlot = InventorySlots[HotbarInventoryTag].Slots.Emplace_GetRef(CreateSlot(HotbarInventoryTag, i));
			OnInventorySlotAddedDelegate.Broadcast(HotbarInventoryTag, NewSlot.Get());
		}
	}
}

	
	/*----------------------------------------------------------------------------
		Hotbar
	----------------------------------------------------------------------------*/
void UCISPlayerInventoryComponent::IncrementSelectedHotbarSlot()
{
	UpdateSelectedHotbarSlot(1);
}

void UCISPlayerInventoryComponent::DecrementSelectedHotbarSlot()
{
	UpdateSelectedHotbarSlot(-1);
}

void UCISPlayerInventoryComponent::FreezeSelectedSlot()
{
	FreezeSelectedSlotCount++;
}

void UCISPlayerInventoryComponent::UnFreezeSelectedSlot()
{
	FreezeSelectedSlotCount--;
	if (FreezeSelectedSlotCount < 0)
	{
		FreezeSelectedSlotCount = 0;
	}
}

void UCISPlayerInventoryComponent::UpdateSelectedHotbarSlot(int32 AdditiveIndex)
{
	if (FreezeSelectedSlotCount > 0) { return; }
	
	const int32 OldHotbarIndex = CurrentSelectedSlotIndex;
	CurrentSelectedSlotIndex += AdditiveIndex;
	
	if (CurrentSelectedSlotIndex > HotbarMaxIndex)
	{
		CurrentSelectedSlotIndex = 0;
	}
	else if (CurrentSelectedSlotIndex < 0)
	{
		CurrentSelectedSlotIndex = HotbarMaxIndex;
	}

	OnSelectedHotbarSlotChangedDelegate.Broadcast(
		this,
		FCISHotbarChangedEvent(OldHotbarIndex, CurrentSelectedSlotIndex)
	);


	if (auto* SelectedHotbarSlot = GetSlot(InventoryDeveloperSettings->HotbarInventoryCategoryTag, CurrentSelectedSlotIndex))
	{
		// TODO: remove previous equiped item (if any)
		
		if (!SelectedHotbarSlot->IsEmpty()
			|| !SelectedHotbarSlot->GetRepresentedItem()->HasTrait(InventoryDeveloperSettings->InventoryEquipableTrait))
		{
			if (const auto* ActorItemFragment = SelectedHotbarSlot->GetRepresentedItem()->GetFragmentFromType<FCISActorInventoryItemFragment>())
			{
				// TODO: update equipped item
			}
		}
	}
}
