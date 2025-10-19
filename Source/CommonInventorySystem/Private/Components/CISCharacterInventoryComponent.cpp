// By hzFishy - 2025 - Do whatever you want with it.


#include "Components/CISCharacterInventoryComponent.h"
#include "Asserts/FUAsserts.h"
#include "Data/CISInventoryDeveloperSettings.h"
#include "Console/FUConsole.h"
#include "Core/CISInventoryItem.h"
#include "Core/CISInventorySlot.h"
#include "Data/Fragments/CISActorInventoryItemFragment.h"


#if FU_WITH_CONSOLE
namespace CIS::Core::Debug
{
	FU_CMD_RUNFUNC_OBJECT_ALL(DumpAllSlotsForCharacters,
		"CIS.Inventory.DumpAllSlotsForCharacters", "Dump info for all slots for all character inventory components",
		UCISCharacterInventoryComponent, DumpAllSlots);

	FU_CMD_RUNFUNC_OBJECT_ALL(DumpCacheInfoForAllCharacters,
		"CIS.Inventory.DumpCacheInfoForAllCharacters", "Dump cache info for all character inventory components",
		UCISCharacterInventoryComponent, DumpCacheInfo);
}
#endif

	
	/*----------------------------------------------------------------------------
		Defaults
	----------------------------------------------------------------------------*/
UCISCharacterInventoryComponent::UCISCharacterInventoryComponent():
	bInitialSelectedHotbarIndexSlotApplied(false),
	CachedHotbarMaxIndex(-1),
	CurrentSelectedSlotIndex(-1),
	FreezeSelectedSlotCount(false)
{}

	
	/*----------------------------------------------------------------------------
		Init
	----------------------------------------------------------------------------*/
void UCISCharacterInventoryComponent::OnInventoryItemDefinitionsLoaded(FCISInventoryItemDefinitionsLoadRequest LoadRequest)
{
	Super::OnInventoryItemDefinitionsLoaded(LoadRequest);

	// init hotbar data, real slots will be createdf by the parent
	if (auto* HotbarData = LoadRequest.Categories.Find(InventoryDeveloperSettings->HotbarInventoryCategoryTag))
	{
		CachedHotbarMaxIndex = HotbarData->CategoryDefinition.SlotDefinitions.Num() - 1;

		if (InitialSelectedHotbarIndexSlot.IsSet())
		{
			UpdateSelectedHotbarSlot(InitialSelectedHotbarIndexSlot.GetValue());
			bInitialSelectedHotbarIndexSlotApplied = true;
		}
	}
}

	
	/*----------------------------------------------------------------------------
		Hotbar
	----------------------------------------------------------------------------*/
void UCISCharacterInventoryComponent::SetHotbarEquipComponent(UPrimitiveComponent* Component)
{
	HotbarEquipComponent = Component;
}

void UCISCharacterInventoryComponent::IncrementSelectedHotbarSlot()
{
	AdditiveUpdateSelectedHotbarSlot(1);
}

void UCISCharacterInventoryComponent::DecrementSelectedHotbarSlot()
{
	AdditiveUpdateSelectedHotbarSlot(-1);
}

void UCISCharacterInventoryComponent::FreezeSelectedSlot()
{
	FreezeSelectedSlotCount++;
}

void UCISCharacterInventoryComponent::UnFreezeSelectedSlot()
{
	FreezeSelectedSlotCount--;
	if (FreezeSelectedSlotCount < 0)
	{
		FreezeSelectedSlotCount = 0;
	}
}

bool UCISCharacterInventoryComponent::IsHoldingActorItem() const
{
	return CurrentHotbarSelection.IsActorSet();
}

void UCISCharacterInventoryComponent::AdditiveUpdateSelectedHotbarSlot(int32 AdditiveIndex)
{
	int32 NewIndex = CurrentSelectedSlotIndex + AdditiveIndex;
	
	if (NewIndex > CachedHotbarMaxIndex)
	{
		NewIndex = 0;
	}
	else if (NewIndex < 0)
	{
		NewIndex = CachedHotbarMaxIndex;
	}
	
	UpdateSelectedHotbarSlot(NewIndex);
}

void UCISCharacterInventoryComponent::UpdateSelectedHotbarSlot(int32 NewIndex)
{
	if (FreezeSelectedSlotCount > 0) { return; }
	
	const int32 OldHotbarIndex = CurrentSelectedSlotIndex;
	CurrentSelectedSlotIndex = NewIndex;
	
	if (auto* OldSelectedHotbarSlot = GetSlot(InventoryDeveloperSettings->HotbarInventoryCategoryTag, OldHotbarIndex))
	{
		OldSelectedHotbarSlot->OnSlotUpdatedDelegate.RemoveDynamic(this, &ThisClass::OnInventorySlotUpdatedCallback);
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

void UCISCharacterInventoryComponent::DeferredSpawnEquipedItem(const TSoftClassPtr<AActor>& SoftActorClass, float ChangeGameTime,
                                                               const FCISAttachInventoryItemFragment& AttachFragment)
{
	if (auto* AM = UAssetManager::GetIfInitialized())
	{
		AM->GetStreamableManager().RequestAsyncLoad(
			SoftActorClass.ToSoftObjectPath(),
			FStreamableDelegate::CreateWeakLambda(this, [this, SoftActorClass, ChangeGameTime, &AttachFragment]
			{
				// see if another request got sent since last time
				// if set time is bigger than us it means that a new request was made while we were processing the async request
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
							AttachFragment.Socket
						);
						
						if (!FU::Utils::IsTransformIdentity(AttachFragment.RelativeSocketTransform))
						{
							SpawnedItemActor->SetActorRelativeTransform(AttachFragment.RelativeSocketTransform);
						}
						
						CurrentHotbarSelection.SetActor(SpawnedItemActor);
					}
				}
			})
		);
	}
}

void UCISCharacterInventoryComponent::SpawnActorForCurrentHotbarSelection()
{
	if (auto* SelectedHotbarSlot = GetSlot(InventoryDeveloperSettings->HotbarInventoryCategoryTag, CurrentSelectedSlotIndex))
	{
		if (SelectedHotbarSlot->GetRepresentedItem()->HasTrait(InventoryDeveloperSettings->InventoryEquipableTrait))
		{
			if (const auto* ActorItemFragment = SelectedHotbarSlot->GetRepresentedItem()->GetFragmentFromType<FCISActorInventoryItemFragment>())
			{
				auto* AttachItemFragment = SelectedHotbarSlot->GetRepresentedItem()->GetFragmentFromType<FCISAttachInventoryItemFragment>();
				
				if (FU_ENSURE_WEAKVALID_MSG(HotbarEquipComponent, "HotbarEquipComponent is invalid")
					&& FU_ENSURE_WEAKNOTNULL_MSG(ActorItemFragment->SoftActorClass, "ActorItemFragment->SoftActorClass is not set")
					&& FU_ENSURE_MSG(AttachItemFragment, "AttachItemFragment wasn't found"))
				{
					// update equipped item
					const float GameTime = GetWorld()->GetRealTimeSeconds();
					CurrentHotbarSelection.SetNewChange(GameTime);
					DeferredSpawnEquipedItem(ActorItemFragment->SoftActorClass, GameTime, *AttachItemFragment);
				}
			}
		}
	}
}

void UCISCharacterInventoryComponent::OnInventorySlotUpdatedCallback(UCISInventorySlot* Slot, const FCISInventorySlotUpdateInfo& UpdateInfo)
{
	// if it was empty , spawn
	if (!CurrentHotbarSelection.IsActorSet() && UpdateInfo.Amount > 0)
	{
		SpawnActorForCurrentHotbarSelection();
	}
}
