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
		"CIS.Inventory.DumpAllSlotsForPlayer", "Dump info for all slots for unique player inventory component",
		UCISPlayerInventoryComponent, DumpAllSlots);

	FU_CMD_OBJECT_ALLRUNFUNC(DumpCacheInfoForPlayer,
		"CIS.Inventory.DumpCacheInfoForPlayer", "Dump cache info for unique player inventory component",
		UCISPlayerInventoryComponent, DumpCacheInfo);
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
{}

	
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

		if (InitialSelectedHotbarIndexSlot.IsSet())
		{
			UpdateSelectedHotbarSlot(CurrentSelectedSlotIndex + 2 + InitialSelectedHotbarIndexSlot.GetValue());
			bInitialSelectedHotbarIndexSlotApplied = true;
		}
	}
}

	
	/*----------------------------------------------------------------------------
		Hotbar
	----------------------------------------------------------------------------*/
void UCISPlayerInventoryComponent::SetHotbarEquipComponent(UPrimitiveComponent* Component)
{
	HotbarEquipComponent = Component;
}

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

void UCISPlayerInventoryComponent::DeferredSpawnEquipedItem(const TSoftClassPtr<AActor>& SoftActorClass, float ChangeGameTime)
{
	if (auto* AM = UAssetManager::GetIfInitialized())
	{
		AM->GetStreamableManager().RequestAsyncLoad(
			SoftActorClass.ToSoftObjectPath(),
			FStreamableDelegate::CreateWeakLambda(this, [this, SoftActorClass, ChangeGameTime]
			{
				// see if another request got sent since last time
				if (CurrentHotbarSelection.GetSetRealTime() <= ChangeGameTime)
				{
					if (auto* LoadedClass = SoftActorClass.Get())
					{
						FActorSpawnParameters SpawnParameters;
						auto* SpawnedItemActor = GetWorld()->SpawnActor<AActor>(LoadedClass, SpawnParameters);

						auto AttachmentTransformRules = FAttachmentTransformRules::SnapToTargetNotIncludingScale;
						SpawnedItemActor->AttachToComponent(
							HotbarEquipComponent.Get(),
							AttachmentTransformRules,
							HotbarEquipSocket
						);
						
						CurrentHotbarSelection.SetActor(SpawnedItemActor);
					}
				}
			})
		);
	}
}

void UCISPlayerInventoryComponent::UpdateSelectedHotbarSlot(int32 AdditiveIndex)
{
	if (FreezeSelectedSlotCount > 0) { return; }
	
	const int32 OldHotbarIndex = CurrentSelectedSlotIndex;
	if (auto* OldSelectedHotbarSlot = GetSlot(InventoryDeveloperSettings->HotbarInventoryCategoryTag, OldHotbarIndex))
	{
		OldSelectedHotbarSlot->OnSlotUpdatedDelegate.RemoveDynamic(this, &ThisClass::OnInventorySlotUpdatedCallback);
	}
	
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
		CurrentHotbarSelection.Clear();
		
		if (!SelectedHotbarSlot->IsEmpty())
		{
			SpawnActorForCurrentHotbarSelection();
		}

		SelectedHotbarSlot->OnSlotUpdatedDelegate.AddUniqueDynamic(this, &ThisClass::OnInventorySlotUpdatedCallback);
	}
}

void UCISPlayerInventoryComponent::SpawnActorForCurrentHotbarSelection()
{
	if (auto* SelectedHotbarSlot = GetSlot(InventoryDeveloperSettings->HotbarInventoryCategoryTag, CurrentSelectedSlotIndex))
	{
		if (SelectedHotbarSlot->GetRepresentedItem()->HasTrait(InventoryDeveloperSettings->InventoryEquipableTrait))
		{
			if (const auto* ActorItemFragment = SelectedHotbarSlot->GetRepresentedItem()->GetFragmentFromType<FCISActorInventoryItemFragment>())
			{
				if (FU_ENSURE_WEAKVALID_MSG(HotbarEquipComponent, "HotbarEquipComponent is invalid")
					&& FU_ENSURE_WEAKNOTNULL_MSG(ActorItemFragment->SoftActorClass, "ActorItemFragment->SoftActorClass is not set"))
				{
					// update equipped item
					const float GameTime = GetWorld()->GetRealTimeSeconds();
					CurrentHotbarSelection.SetNewChange(GameTime);
					DeferredSpawnEquipedItem(ActorItemFragment->SoftActorClass, GameTime);
				}
			}
		}
	}
}

void UCISPlayerInventoryComponent::OnInventorySlotUpdatedCallback(UCISInventorySlot* Slot, const FCISInventorySlotUpdateInfo& UpdateInfo)
{
	// if it was empty , spawn
	if (!CurrentHotbarSelection.IsActorSet() && UpdateInfo.Amount > 0)
	{
		SpawnActorForCurrentHotbarSelection();
	}
}
