// By hzFishy - 2025 - Do whatever you want with it.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "Interfaces/CISInventoryDefinitionInterface.h"
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

/** Used to hold temporary vars while we move slot items */
/*struct COMMONINVENTORYSYSTEM_API FCISInventorySlotMoveTransaction
{
	FCISInventorySlotMoveTransaction(UCISInventorySlot* SourceSlot, int32 MovedCount);

	
};*/


COMMONINVENTORYSYSTEM_API DECLARE_LOG_CATEGORY_EXTERN(LogCISInventory, Log, All);
#define CIS_LOG_INV_W(FORMAT, ...) _FU_LOG_OBJECT_W(LogCISInventory, FORMAT, __VA_ARGS__)


UCLASS(ClassGroup=(CommonInventorySystem), DisplayName="Base Inventory Component")
class COMMONINVENTORYSYSTEM_API UCISBaseInventoryComponent : public UActorComponent
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
	
	
	/*----------------------------------------------------------------------------
		Defaults
	----------------------------------------------------------------------------*/
public:
	UCISBaseInventoryComponent();

	virtual void OnRegister() override;

	
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


	/*----------------------------------------------------------------------------
		Operations
	----------------------------------------------------------------------------*/
public:
	/**
	 * Called from UI when user drag and drops slot on another one.
	 * This could be
	 * - moving the items from the source slot to the empty target one
	 * - swapping the items around if both slot don't have same item id
	 * - if same id stacking the items on the target one
	 */
	UFUNCTION(BlueprintCallable, Category="CommonInventorySystem")
	void RequestMove(FGameplayTag SourceSlotCategory, int32 SourceSlotIndex,
		FGameplayTag TargetSlotCategory, int32 TargetSlotIndex);


	/*----------------------------------------------------------------------------
		Utilities
	----------------------------------------------------------------------------*/
public:
	UCISInventorySlot* GetSlot(FGameplayTag SlotCategory, int32 SlotIndex);

	
	/*----------------------------------------------------------------------------
		Debug
	----------------------------------------------------------------------------*/
public:
	void DumpAllSlots();
};
