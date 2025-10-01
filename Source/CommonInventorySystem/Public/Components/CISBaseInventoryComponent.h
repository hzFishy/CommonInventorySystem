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

USTRUCT(BlueprintType)
struct COMMONINVENTORYSYSTEM_API FCISInventorySlotCategory
{
	GENERATED_BODY()

	FCISInventorySlotCategory();
	FCISInventorySlotCategory(int32 Size);

	UPROPERTY(BlueprintReadOnly)
	TArray<TObjectPtr<UCISInventorySlot>> Slots;
};

/** Cached info per item in inventory */
struct COMMONINVENTORYSYSTEM_API FCISInventoryItemInfo
{
	FCISInventoryItemInfo();
	FCISInventoryItemInfo(int32 InAmount);

	int32 Amount;
};

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

	TSubclassOf<UCISInventorySlot> InventorySlotClass;

	bool bSlotsCreated;
	
	UPROPERTY()
	TMap<FGameplayTag, FCISInventorySlotCategory> InventorySlots;
	TMap<FGameplayTag, FCISInventoryItemInfo> CachedInventoryItemInfo;
	
	
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
	
	virtual void OnInventoryItemDefinitionsLoaded(TSoftClassPtr<UCISInventorySlot> SoftInventorySlotClass, TArray<FCISInventorySlotDefinition> SlotDefinitions);

	virtual UCISInventorySlot* CreateSlot(FGameplayTag SlotCategory, int32 SlotIndex);
	
	virtual UCISInventorySlot* CreateSlotFromDefinition(FGameplayTag SlotCategory, int32 SlotIndex, const FCISInventorySlotDefinition& SlotDefinition);

	void DeferredCreateItemsFromDefinition(UCISInventorySlot* Slot, int32 ItemCount, UCISInventoryItemDefinition* ItemDefinition);

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

	void RequestAdd(const FCISInventoryAddRequest& AddRequest);

	
	/*----------------------------------------------------------------------------
		Utilities
	----------------------------------------------------------------------------*/
public:
	UCISInventorySlot* GetSlot(FGameplayTag SlotCategory, int32 SlotIndex);
	
	UCISInventorySlot* GetSlotForItemTag(FGameplayTag SlotCategory, FGameplayTag ItemTag);

protected:
	void AddItemsToCachedInfo(FGameplayTag ItemTag, int32 Amount);
	
	
	/*----------------------------------------------------------------------------
		Debug
	----------------------------------------------------------------------------*/
public:
	void DumpAllSlots();
};
