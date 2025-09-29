// By hzFishy - 2025 - Do whatever you want with it.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Object.h"
#include "CISInventorySlot.generated.h"
class UCISInventoryItem;

USTRUCT(BlueprintType)
struct COMMONINVENTORYSYSTEM_API FCISInventorySlotUpdateInfo
{
	GENERATED_BODY()

	FCISInventorySlotUpdateInfo();
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCISOnInventorySlotUpdatedSignature,
	UCISInventorySlot*, Slot,
	const FCISInventorySlotUpdateInfo&, UpdateInfo
);


/**
 * A living slot of an inventory.
 * Outer is the owner inventory component.
 */
UCLASS(ClassGroup=CommonInventorySystem, DisplayName="Inventory Slot", BlueprintType, Blueprintable)
class COMMONINVENTORYSYSTEM_API UCISInventorySlot : public UObject
{
	GENERATED_BODY()

	
	/*----------------------------------------------------------------------------
		Properties
	----------------------------------------------------------------------------*/
public:
	UPROPERTY(BlueprintAssignable, DisplayName="On Slot Updated")
	FCISOnInventorySlotUpdatedSignature OnSlotUpdatedDelegate;
	
protected:
	FGameplayTag SlotCategory;

	/** Relative to the inventory component category */
	int32 SlotIndex;

	/**
	 * The items in this slot, all items should be from the same class.
	 * The count of items for this slot is the size of this array
	 */
	UPROPERTY()
	TArray<TObjectPtr<UCISInventoryItem>> Items;

	
	/*----------------------------------------------------------------------------
		Defaults
	----------------------------------------------------------------------------*/
public:
	UCISInventorySlot();

	
	/*----------------------------------------------------------------------------
		Core
	----------------------------------------------------------------------------*/
public:
	void InitSlot(FGameplayTag InSlotCategory, int32 InSlotIndex);

	/**
	 * Add item to items array.
	 * @param bCallUpdate do we call CallUpdate after
	 */
	void AddItem(UCISInventoryItem* NewItem, bool bCallUpdate);

	/**
	 * Add the items to items array.
	 * @param bCallUpdate do we call CallUpdate after adding all items
	 */
	void AddItems(const TArray<UCISInventoryItem*>& NewItems, bool bCallUpdate);
	
	/**
	 * Removes all items in the array.
	 * @param bCallUpdate do we call CallUpdate after removing all items
	 */
	void ClearAllItems(bool bCallUpdate);

	void CallUpdate();
	
	UFUNCTION(BlueprintPure)
	bool IsEmpty() const { return Items.IsEmpty(); };

	UFUNCTION(BlueprintPure)
	FGameplayTag GetRepresentedCategoryTag() const { return SlotCategory;};
	
	UFUNCTION(BlueprintPure)
	int32 GetSlotIndex() const { return SlotIndex; };
	
	UFUNCTION(BlueprintPure)
	int32 GetItemCount() const { return Items.Num(); };

	UFUNCTION(BlueprintPure)
	FGameplayTag GetRepresentedItemTag() const;
	
	const TArray<UCISInventoryItem*>& GetItems();
	TArray<UCISInventoryItem*> GetItemsCopy();

	/** Since all items in this slot should be equal, the first valid item in the list represents the other ones */
	UFUNCTION(BlueprintPure)
	UCISInventoryItem* GetRepresentedItem();
};
