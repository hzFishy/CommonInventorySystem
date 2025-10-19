// By hzFishy - 2025 - Do whatever you want with it.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "Interfaces/CISInventoryDefinitionInterface.h"
#include "Interfaces/CTItemProviderInterface.h"
#include "Logging/FULogging.h"
#include "CISBaseInventoryComponent.generated.h"
class UCISInventoryItem;
class UCISInventoryDeveloperSettings;
class UCISInventorySlot;
class UCISInventoryItemDefinition;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCISOnInventorySlotAddedSignature,
	FGameplayTag, Category,
	UCISInventorySlot*, NewSlot
);

	
	/*----------------------------------------------------------------------------
		Slot
	----------------------------------------------------------------------------*/
USTRUCT(BlueprintType)
struct COMMONINVENTORYSYSTEM_API FCISInventorySlotCategory
{
	GENERATED_BODY()

	FCISInventorySlotCategory();
	FCISInventorySlotCategory(int32 Size);

	UPROPERTY(BlueprintReadOnly)
	TArray<TObjectPtr<UCISInventorySlot>> Slots;
};

struct COMMONINVENTORYSYSTEM_API FCISInventorySlotIdentity
{
	FCISInventorySlotIdentity(UCISInventorySlot* Slot);
	FCISInventorySlotIdentity(FGameplayTag InSlotTag, int32 InSlotIndex);

	bool operator==(const FCISInventorySlotIdentity& Other) const;
	
	FGameplayTag SlotCategoryTag;
	int32 SlotIndex;
};

	
	/*----------------------------------------------------------------------------
		Cache Info
	----------------------------------------------------------------------------*/
struct COMMONINVENTORYSYSTEM_API FCISInventoryItemCacheInfoSlotDataEntry
{
	FCISInventoryItemCacheInfoSlotDataEntry(int32 InAmount);
	
	/** How much of this item we got in the inventory? */
	int32 Amount;
};

/** Cache entry per slot cateogry */
struct COMMONINVENTORYSYSTEM_API FCISInventoryItemCacheInfoSlotData
{
	FCISInventoryItemCacheInfoSlotData(int32 InSlotIndex, int32 InAmount);

	void AddSlot(int32 SlotIndexIndex, const FCISInventoryItemCacheInfoSlotDataEntry& EntryData);
	
	void AddAmountToSlot(int32 SlotIndex, int32 Amount);
	
	void RemoveSlot(int32 SlotIndex);

	const FCISInventoryItemCacheInfoSlotDataEntry& GetEntryData(int32 Index) const;
	
	int32 GetSlotCount() const { return Entries.Num(); }
	
	bool IsEmpty() const { return Entries.IsEmpty(); }

	bool HasSlotIndex(int32 Index) const { return Entries.Contains(Index); }
	
	const TMap<int32, FCISInventoryItemCacheInfoSlotDataEntry>& GetEntries() const { return Entries; }
	
protected:
	/** Key is the slot index in the parent category */
	TMap<int32, FCISInventoryItemCacheInfoSlotDataEntry> Entries;
};

/** Cached info per item tag in inventory */
struct COMMONINVENTORYSYSTEM_API FCISInventoryItemCacheInfo
{
	FCISInventoryItemCacheInfo(const FCISInventorySlotIdentity& InSlot, int32 InAmount);

	
	void AddAmountForSlot(const FCISInventorySlotIdentity& InSlot, int32 InAmount);
	void RemoveAmountForSlot(const FCISInventorySlotIdentity& InSlot, int32 InAmount);
	void RemoveSlot(const FCISInventorySlotIdentity& InSlot);

	const TMap<FGameplayTag, FCISInventoryItemCacheInfoSlotData>& GetSlots();
	const FCISInventoryItemCacheInfoSlotData& GetDataForSlotCategory(const FCISInventorySlotIdentity& InSlot) const;
	const FCISInventoryItemCacheInfoSlotDataEntry& GetDataForSlotIndex(const FCISInventorySlotIdentity& InSlot) const;
	
protected:
	/** Data per category where this item is used */
	TMap<FGameplayTag, FCISInventoryItemCacheInfoSlotData> Slots;
};


	/*----------------------------------------------------------------------------
		Load Request
	----------------------------------------------------------------------------*/
struct COMMONINVENTORYSYSTEM_API FCISInventoryItemDefinitionsLoadRequestSlotCategory
{
	FCISInventoryItemDefinitionsLoadRequestSlotCategory();

	FGameplayTag CategoryTag;
	
	FCISInventoryCategoryDefinition CategoryDefinition;
};

struct COMMONINVENTORYSYSTEM_API FCISInventoryItemDefinitionsLoadRequest
{
	FCISInventoryItemDefinitionsLoadRequest();

	TMap<FGameplayTag, FCISInventoryItemDefinitionsLoadRequestSlotCategory> Categories;
};

	
	/*----------------------------------------------------------------------------
		Remove Request
	----------------------------------------------------------------------------*/
struct COMMONINVENTORYSYSTEM_API FCISInventoryRemoveRequestItem
{
	FCISInventoryRemoveRequestItem();
	FCISInventoryRemoveRequestItem(FGameplayTag InTag, int32 InAmount);

	FGameplayTag Tag;
	int32 Amount;
};

struct COMMONINVENTORYSYSTEM_API FCISInventoryRemoveRequest
{
	FCISInventoryRemoveRequest();
	FCISInventoryRemoveRequest(const FCTItemProviderCraftQuery& CraftQuery, const FCTItemProviderItemSearchQueryResult& InItemSearchQueryResult);

	TArray<FCISInventoryRemoveRequestItem> Items;
	const FCTItemProviderItemSearchQueryResult* ItemSearchQueryResult;
};

	
	/*----------------------------------------------------------------------------
		Add Request
	----------------------------------------------------------------------------*/
struct COMMONINVENTORYSYSTEM_API FCISInventoryAddRequestItem
{
	FCISInventoryAddRequestItem();
	FCISInventoryAddRequestItem(FGameplayTag InTag, int32 InAmount);

	FGameplayTag Tag;
	int32 Amount;
};

struct COMMONINVENTORYSYSTEM_API FCISInventoryAddRequest
{
	FCISInventoryAddRequest();
	FCISInventoryAddRequest(const FCTItemProviderCraftQuery& CraftQuery, FGameplayTag InSlotCateogryTag);

	TArray<FCISInventoryAddRequestItem> Items;
	FGameplayTag SlotCateogryTag;
};


USTRUCT(BlueprintType, DisplayName="Inventory Add Request Blueprint Entry")
struct COMMONINVENTORYSYSTEM_API FCISInventoryAddRequestBlueprintEntry
{
	GENERATED_BODY()
	
	FCISInventoryAddRequestBlueprintEntry();

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag ItemTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UCISInventoryItemDefinition> SoftItemDefinition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Amount;
};

USTRUCT(BlueprintType, DisplayName="Inventory Add Request Blueprint")
struct COMMONINVENTORYSYSTEM_API FCISInventoryAddRequestBlueprint
{
	GENERATED_BODY()
	
	FCISInventoryAddRequestBlueprint();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FCISInventoryAddRequestBlueprintEntry> Entries;
};


COMMONINVENTORYSYSTEM_API DECLARE_LOG_CATEGORY_EXTERN(LogCISInventory, Log, All);
#define CIS_LOG_INV_W(FORMAT, ...) _FU_LOG_OBJECT_W(LogCISInventory, FORMAT, __VA_ARGS__)


UCLASS(ClassGroup=(CommonInventorySystem), DisplayName="Base Inventory Component")
class COMMONINVENTORYSYSTEM_API UCISBaseInventoryComponent : public UActorComponent, public ICTItemProviderInterface
{
	GENERATED_BODY()

	
	/*----------------------------------------------------------------------------
		Properties
	----------------------------------------------------------------------------*/
public:
	UPROPERTY(BlueprintAssignable, DisplayName="On Inventory Slot Added")
	FCISOnInventorySlotAddedSignature OnInventorySlotAddedDelegate;
	
protected:
	UPROPERTY(EditAnywhere, Category="CommonInventorySystem", meta=(AllowedClasses="/Script/CommonInventorySystem.CISInventoryDefinitionInterface"))
	TSoftObjectPtr<UPrimaryDataAsset> InventoryPresetDefinition;


	TWeakObjectPtr<const UCISInventoryDeveloperSettings> InventoryDeveloperSettings;

	TMap<FGameplayTag, TSubclassOf<UCISInventorySlot>> InventorySlotClassPerCategory;

	bool bSlotsCreated;
	
	UPROPERTY()
	TMap<FGameplayTag, FCISInventorySlotCategory> InventorySlots;
	TMap<FGameplayTag, FCISInventoryItemCacheInfo> CachedInventoryItemInfo;
	
	
	/*----------------------------------------------------------------------------
		Defaults
	----------------------------------------------------------------------------*/
public:
	UCISBaseInventoryComponent();

	virtual void OnRegister() override;

	virtual bool SearchItems(const FCTItemProviderItemSearchQuery& Query, FCTItemProviderItemSearchQueryResult& QueryResult) override;

	virtual bool CraftRecipe(const FCTItemProviderCraftQuery& CraftQuery) override;
	
	
	/*----------------------------------------------------------------------------
		Init
	----------------------------------------------------------------------------*/
public:
	void InitFromPresetDefinition();

	UFUNCTION(BlueprintPure, Category="CommonInventorySystem")
	bool AreSlotsCreated() const { return bSlotsCreated; };

	UFUNCTION(BlueprintCallable, Category="CommonInventorySystem")
	const TMap<FGameplayTag, FCISInventorySlotCategory>& GetAllSlots() const { return InventorySlots; };

protected:
	virtual void OnInventoryPresetDefinitionLoaded();
	
	virtual void OnInventoryItemDefinitionsLoaded(FCISInventoryItemDefinitionsLoadRequest LoadRequest);

	virtual void OnInitialSlotsDoneInitialization();
	
	virtual UCISInventorySlot* CreateSlot(FGameplayTag SlotCategory, int32 SlotIndex);
	
	virtual UCISInventorySlot* CreateSlotFromDefinition(FGameplayTag SlotCategory, int32 SlotIndex, const FCISInventorySlotDefinition& SlotDefinition);

	void DeferredCreateItemsFromDefinition(UCISInventorySlot* Slot, int32 ItemCount, UCISInventoryItemDefinition* ItemDefinition);
	
	void CreateItemsFromLoadedDefinition(UCISInventorySlot* Slot, int32 ItemCount, UCISInventoryItemDefinition* ItemDefinition);

	/** Get the definition of an item from its tag and add it to a given slot */
	void DeferredCreateItemsFromTag(UCISInventorySlot* Slot, int32 ItemCount, FGameplayTag ItemTag);

	
	/*----------------------------------------------------------------------------
		Operations
	----------------------------------------------------------------------------*/
public:
	/**
	 * Global function for moving items from a source slot to the target slot.
	 * 
	 * For example this would typically be called from UI when the user drag and drops a slot on another one.
	 * This could mean:
	 * - moving the items from the source slot to the empty target one
	 * - swapping the items around if both slot don't have same item id
	 * - if same id stacking the items on the target one
	 */
	UFUNCTION(BlueprintCallable, Category="CommonInventorySystem")
	void RequestMove(FGameplayTag SourceSlotCategory, int32 SourceSlotIndex,
		FGameplayTag TargetSlotCategory, int32 TargetSlotIndex);

	void RequestRemove(const FCISInventoryRemoveRequest& RemoveRequest);

	/** Add requested items to available slots */
	UFUNCTION(BlueprintCallable, Category="CommonInventorySystem", DisplayName="Request Add")
	void K2_RequestAdd(FCISInventoryAddRequestBlueprint AddRequestBlueprint);
	
	void RequestAdd(const FCISInventoryAddRequest& AddRequest);
	
	
	/*----------------------------------------------------------------------------
		Utilities
	----------------------------------------------------------------------------*/
public:
	UCISInventorySlot* GetSlot(FGameplayTag SlotCategory, int32 SlotIndex);
	
	UCISInventorySlot* GetSlotForItemTag(FGameplayTag SlotCategory, FGameplayTag ItemTag);
	
	/** Check in all categories, if not found fallback to given category */
	UCISInventorySlot* GetSlotForItemTagAnyCategory(FGameplayTag FallbackSlotCategory, FGameplayTag ItemTag);

protected:
	void AddItemsToCachedInfo(UCISInventorySlot* Slot, FGameplayTag ItemTag, int32 Amount);
	
	void RemoveItemsToCachedInfo(UCISInventorySlot* Slot, FGameplayTag ItemTag, int32 Amount);

	void RemoveAllItemsToCachedInfo(UCISInventorySlot* Slot);
	
	
	/*----------------------------------------------------------------------------
		Debug
	----------------------------------------------------------------------------*/
public:
	void DumpAllSlots();

	void DumpCacheInfo();
};
