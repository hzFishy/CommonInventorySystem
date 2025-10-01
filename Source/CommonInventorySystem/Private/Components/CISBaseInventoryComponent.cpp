// By hzFishy - 2025 - Do whatever you want with it.


#include "Components/CISBaseInventoryComponent.h"
#include "Asserts/FUAsserts.h"
#include "Core/CISInventoryItem.h"
#include "Core/CISInventorySlot.h"
#include "Data/CISInventoryDeveloperSettings.h"
#include "Data/Definitions/CISInventoryItemDefinition.h"
#include "Interfaces/CISInventoryDefinitionInterface.h"
#include "Utility/FUUtilities.h"
#include "Console/FUConsole.h"


#if FU_WITH_CONSOLE
namespace CIS::Core::Debug
{
	FU_CMD_OBJECT_ALLRUNFUNC(DumpAllSlotsForAll,
		"CIS.Inventory.DumpAllSlotsForAll", "Dump info for all slots for all base inventory components",
		UCISBaseInventoryComponent, DumpAllSlots);
}
#endif


FCISInventorySlotCategory::FCISInventorySlotCategory() {}

FCISInventorySlotCategory::FCISInventorySlotCategory(int32 Size)
{
	Slots.Reserve(Size);
}


FCISInventoryItemInfo::FCISInventoryItemInfo():
	Amount(0)
{}

FCISInventoryItemInfo::FCISInventoryItemInfo(int32 InAmount): Amount(InAmount)
{}


DEFINE_LOG_CATEGORY(LogCISInventory);

	
	/*----------------------------------------------------------------------------
		Defaults
	----------------------------------------------------------------------------*/
UCISBaseInventoryComponent::UCISBaseInventoryComponent():
	bSlotsCreated(false)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCISBaseInventoryComponent::OnRegister()
{
	Super::OnRegister();

#if WITH_EDITOR
	FU_UTILS_EDITOR_RETURN_NOTGAMEWORLD
#endif

	InventoryDeveloperSettings = GetDefault<UCISInventoryDeveloperSettings>();
	
	InitFromPresetDefinition();
}

bool UCISBaseInventoryComponent::SearchItems(const FCTItemProviderItemSearchQuery& Query, FCTItemProviderItemSearchQueryResult& QueryResult)
{
	TMap<FGameplayTag, const FCTItemProviderItemSearchQueryItem> QuickSearchMap;
	Query.GetQuickSearchMap(QuickSearchMap);

	// results per item
	TMap<FGameplayTag, FCTItemProviderItemSearchQueryResultItem> QuickResultMap;
	
	for (auto& SlotCategory : InventorySlots)
	{
		// we dont search in hotbar
		if (SlotCategory.Key == InventoryDeveloperSettings->HotbarInventoryCategoryTag) { continue; }

		for (auto& Slot : SlotCategory.Value.Slots)
		{
			// skip if empty
			if (Slot->IsEmpty()) { continue; }
			
			// check if this slots contains an item we are interested in
			auto CurrentSlotItemTag = Slot->GetRepresentedItemTag();
			if (auto* SearchQueryEntry = QuickSearchMap.Find(CurrentSlotItemTag))
			{
				if (auto* QuickResultEntry = QuickResultMap.Find(CurrentSlotItemTag))
				{
					FCTItemProviderItemSearchQueryResultItemSlot ResultSlot;
					ResultSlot.SlotCategoryTag = SlotCategory.Key;
					ResultSlot.SlotIndex = Slot->GetSlotIndex();
					ResultSlot.FoundItemAmount = Slot->GetItemCount();

					QuickResultEntry->FoundSlots.Emplace(ResultSlot);
				}
				else
				{
					FCTItemProviderItemSearchQueryResultItem ResultItem;
					
					FCTItemProviderItemSearchQueryResultItemSlot ResultSlot;
					ResultSlot.SlotCategoryTag = SlotCategory.Key;
					ResultSlot.SlotIndex = Slot->GetSlotIndex();
					ResultSlot.FoundItemAmount = Slot->GetItemCount();

					ResultItem.FoundSlots.Emplace(ResultSlot);
					
					QuickResultMap.Add(CurrentSlotItemTag, ResultItem);
				}
			}
		}
	}

	QuickResultMap.GenerateValueArray(QueryResult.Results);

	return QueryResult.FoundAllItems();
}


bool UCISBaseInventoryComponent::CraftRecipe(const FCTItemProviderCraftQuery& CraftQuery)
{
	// step 1: see if we got all required items
	auto SearchQuery = FCTItemProviderItemSearchQuery(CraftQuery);
	FCTItemProviderItemSearchQueryResult SearchQueryResult;
	const bool bFoundAllItems = SearchItems(SearchQuery, SearchQueryResult);
	if (!bFoundAllItems) { return false; }
	
	// step 2: if yes, consume the input items and produce the output items

	return true;
}

	
	/*----------------------------------------------------------------------------
		Init
	----------------------------------------------------------------------------*/
void UCISBaseInventoryComponent::InitFromPresetDefinition()
{
	if (InventoryPresetDefinition.IsNull()) { return; }
	
	if (auto* AM = UAssetManager::GetIfInitialized())
	{
		AM->GetStreamableManager().RequestAsyncLoad(
			InventoryPresetDefinition.ToSoftObjectPath(),
			FStreamableDelegate::CreateUObject(this, &ThisClass::OnInventoryPresetDefinitionLoaded),
			FStreamableManager::AsyncLoadHighPriority
		);
	}
}

void UCISBaseInventoryComponent::OnInventoryPresetDefinitionLoaded()
{
	auto* PresetDefinitionObject = InventoryPresetDefinition.Get();
	if (!FU_ENSURE_VALID(PresetDefinitionObject)) { return; }

	auto* PresetDefinitionInterface = Cast<ICISInventoryDefinitionInterface>(PresetDefinitionObject);
	if (!FU_ENSURE(PresetDefinitionInterface)) { return; }
	
	// load item definitions
	if (auto* AM = UAssetManager::GetIfInitialized())
	{
		TArray<FCISInventorySlotDefinition> SlotDefinitions;
		PresetDefinitionInterface->GetInventorySlotDefinitions(SlotDefinitions);
	
		TArray<FSoftObjectPath> Paths;
		Paths.Reserve(SlotDefinitions.Num());
		for (auto& SoftSlotDef : SlotDefinitions)
		{
			if (!SoftSlotDef.SoftItemDefinition.IsNull())
			{
				Paths.Emplace(SoftSlotDef.SoftItemDefinition.ToSoftObjectPath());
			}
		}

		// also load slot class
		Paths.Emplace(PresetDefinitionInterface->GetInventorySlotClass().ToSoftObjectPath());
		
		AM->GetStreamableManager().RequestAsyncLoad(
			Paths,
			FStreamableDelegate::CreateUObject(this, &ThisClass::OnInventoryItemDefinitionsLoaded,
				PresetDefinitionInterface->GetInventorySlotClass(),SlotDefinitions),
			FStreamableManager::AsyncLoadHighPriority
		);
	}
}

void UCISBaseInventoryComponent::OnInventoryItemDefinitionsLoaded(TSoftClassPtr<UCISInventorySlot> SoftInventorySlotClass, TArray<FCISInventorySlotDefinition> SlotDefinitions)
{
	InventorySlotClass = SoftInventorySlotClass.Get();
	
	// init default slots
	auto DefaultInventoryTag = InventoryDeveloperSettings->DefaultInventoryCategoryTag;
	InventorySlots.Emplace(DefaultInventoryTag, FCISInventorySlotCategory(SlotDefinitions.Num()));
	for (auto i = 0; i < SlotDefinitions.Num(); i++)
	{
		// this will also creates the item in deferred if necessary
		auto& SlotDefinition = SlotDefinitions[i];
		auto* ItemDefinition = SlotDefinition.SoftItemDefinition.Get();
		auto& NewSlot = InventorySlots[DefaultInventoryTag].Slots.Emplace_GetRef(CreateSlotFromDefinition(DefaultInventoryTag, i, SlotDefinition));
		OnInventorySlotAddedDelegate.Broadcast(DefaultInventoryTag, NewSlot.Get());
	}
	bSlotsCreated = true;
}

UCISInventorySlot* UCISBaseInventoryComponent::CreateSlot(FGameplayTag SlotCategory, int32 SlotIndex)
{
	auto* NewSlot = NewObject<UCISInventorySlot>(
		this, 
		InventorySlotClass
	);

	NewSlot->InitSlot(SlotCategory, SlotIndex);

	return NewSlot;
}

UCISInventorySlot* UCISBaseInventoryComponent::CreateSlotFromDefinition(FGameplayTag SlotCategory, int32 SlotIndex, const FCISInventorySlotDefinition& SlotDefinition)
{
	auto* NewSlot = CreateSlot(SlotCategory, SlotIndex);

	auto* ItemDefinition = SlotDefinition.SoftItemDefinition.Get();
	if (IsValid(ItemDefinition))
	{
		DeferredCreateItemsFromDefinition(NewSlot, SlotDefinition.Count, ItemDefinition);
	}

	return NewSlot;
}

void UCISBaseInventoryComponent::DeferredCreateItemsFromDefinition(UCISInventorySlot* Slot, int32 ItemCount, UCISInventoryItemDefinition* ItemDefinition)
{
	UAssetManager::GetIfInitialized()->GetStreamableManager().RequestAsyncLoad(
		ItemDefinition->ItemClass.ToSoftObjectPath(),
		FStreamableDelegate::CreateWeakLambda(this, [this, Slot, ItemCount, ItemDefinition]()
		{
			for (int32 i = 0; i < ItemCount; i++)
			{
				auto* NewItem = NewObject<UCISInventoryItem>(Slot, ItemDefinition->ItemClass.Get());
				NewItem->InitFromDefinition(ItemDefinition);
				Slot->AddItem(NewItem, false);
			}

			AddItemsToCachedInfo(Slot->GetRepresentedItemTag(), ItemCount);
			Slot->CallUpdate();
		}),
		FStreamableManager::AsyncLoadHighPriority
	);
}

	
	/*----------------------------------------------------------------------------
		Operations
	----------------------------------------------------------------------------*/
void UCISBaseInventoryComponent::RequestMove(FGameplayTag SourceSlotCategory, int32 SourceSlotIndex,
	FGameplayTag TargetSlotCategory, int32 TargetSlotIndex)
{
	// get the slots from the category and index
	auto* SourceSlot = GetSlot(SourceSlotCategory, SourceSlotIndex);
	auto* TargetSlot = GetSlot(TargetSlotCategory, TargetSlotIndex);
	if (!FU_ENSURE_VALID(SourceSlot) || !FU_ENSURE_VALID(TargetSlot)) { return; }
	if (!FU_ENSURE(SourceSlot != TargetSlot)) { return; }
	if (!FU_ENSURE(!SourceSlot->IsEmpty())) { return; }

	// case 1: target slot is empty
	if (TargetSlot->IsEmpty())
	{
		// move objects from source to target
		auto SourceItems = SourceSlot->GetItemsCopy();
		SourceSlot->ClearAllItems(true);
		TargetSlot->AddItems(SourceItems, true);
	}
	// case 2: swapping
	else if (SourceSlot->GetRepresentedItemTag() != TargetSlot->GetRepresentedItemTag())
	{
		// get copy of items
		auto SourceItems = SourceSlot->GetItemsCopy();
		auto TargetItems = TargetSlot->GetItemsCopy();

		// clear slots
		SourceSlot->ClearAllItems(false);
		TargetSlot->ClearAllItems(false);
		
		// move objects
		SourceSlot->AddItems(TargetItems, true);
		TargetSlot->AddItems(SourceItems, true);
	}
	// case3: stack
	else
	{
		// get source objects
		auto SourceItems = SourceSlot->GetItemsCopy();
		TargetSlot->AddItems(SourceItems, true);
		SourceSlot->ClearAllItems(true);
	}
}

	
	/*----------------------------------------------------------------------------
		Utilities
	----------------------------------------------------------------------------*/
UCISInventorySlot* UCISBaseInventoryComponent::GetSlot(FGameplayTag SlotCategory, int32 SlotIndex)
{
	if (auto* Entry = InventorySlots.Find(SlotCategory))
	{
		if (Entry->Slots.IsValidIndex(SlotIndex))
		{
			return Entry->Slots[SlotIndex].Get();
		}
	}

	return nullptr;
}

void UCISBaseInventoryComponent::AddItemsToCachedInfo(FGameplayTag ItemTag, int32 Amount)
{
	if (auto* Entry = CachedInventoryItemInfo.Find(ItemTag))
	{
		Entry->Amount += Amount;
	}
	else
	{
		CachedInventoryItemInfo.Add(ItemTag, FCISInventoryItemInfo(Amount));
	}
}


/*----------------------------------------------------------------------------
		Debug
	----------------------------------------------------------------------------*/
void UCISBaseInventoryComponent::DumpAllSlots()
{
	for (auto& Pair : InventorySlots)
	{
		auto& Category = Pair.Key;
		auto& Slots = Pair.Value.Slots;
		CIS_LOG_INV_W("Slot Cateogry: {0}, Count: {1}", Category.ToString(), Slots.Num());
		for (const auto& Slot : Slots)
		{
			CIS_LOG_INV_W("		Slot [{0}]: Count: {1}, Id: {2}",
				Slot->GetSlotIndex(),
				Slot->GetItemCount(),
				Slot->GetRepresentedItemTag().ToString()
			);
			if (!Slot->IsEmpty())
			{
				for (const auto& Item : Slot->GetItems())
				{
					CIS_LOG_INV_W("			Item: {0}, Fragments: {1} ({2})",
						FU::Utils::GetObjectDetailedName(Item),
						Item->GetFragmentCount(),
						Item->GetFragmentsDebugString()
					);
				}
			}
		}
	}
}
