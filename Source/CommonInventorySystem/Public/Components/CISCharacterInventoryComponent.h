// By hzFishy - 2025 - Do whatever you want with it.

#pragma once

#include "CoreMinimal.h"
#include "CISBaseInventoryComponent.h"
#include "Core/CISInventorySlot.h"
#include "Data/CISInventoryTypes.h"
#include "Data/Fragments/CISAttachInventoryItemFragment.h"
#include "CISCharacterInventoryComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCISOnCharacterInventorySelectedHotbarSlotChangedSignature,
	UCISCharacterInventoryComponent*, CharacterInventoryComponent,
	const FCISHotbarChangedEvent&, HotbarChangedEvent
);


/**
 * Inventory Component for characters (AIs, Player, ...).
 * 
 * The main extra feature it has is hotbar slots and management.
 */
UCLASS(ClassGroup=(CommonInventorySystem), DisplayName="Character Inventory Component", meta=(BlueprintSpawnableComponent))
class COMMONINVENTORYSYSTEM_API UCISCharacterInventoryComponent : public UCISBaseInventoryComponent
{
	GENERATED_BODY()

	
	/*----------------------------------------------------------------------------
		Properties
	----------------------------------------------------------------------------*/
public:
	UPROPERTY(BlueprintAssignable, DisplayName="On Selected Hotbar Slot Changed")
	FCISOnCharacterInventorySelectedHotbarSlotChangedSignature OnSelectedHotbarSlotChangedDelegate;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="CommonInventorySystem")
	TOptional<int32> InitialSelectedHotbarIndexSlot;

	
	/** The component actor items will attach to, usually a Skeletal Mesh Component */
	TWeakObjectPtr<UPrimitiveComponent> HotbarEquipComponent;
	bool bInitialSelectedHotbarIndexSlotApplied;
	int32 CachedHotbarMaxIndex;
	int32 CurrentSelectedSlotIndex;
	int32 FreezeSelectedSlotCount;
	/** Data stored regarding the selected hotbar */
	FCISInventoryHotbarSelection CurrentHotbarSelection;
	
	
	/*----------------------------------------------------------------------------
		Defaults
	----------------------------------------------------------------------------*/
public:
	UCISCharacterInventoryComponent();

	
	/*----------------------------------------------------------------------------
		Init
	----------------------------------------------------------------------------*/
protected:
	virtual void OnInventoryItemDefinitionsLoaded(FCISInventoryItemDefinitionsLoadRequest LoadRequest
	) override;

	
	/*----------------------------------------------------------------------------
		Hotbar
	----------------------------------------------------------------------------*/
public:
	/** The component actor items will attach to, usually a Skeletal Mesh Component */
	UFUNCTION(BlueprintCallable, Category="CommonInventorySystem")
	void SetHotbarEquipComponent(UPrimitiveComponent* Component);
	
	UFUNCTION(BlueprintPure, Category="CommonInventorySystem")
	bool IsInitialSelectedHotbarIndexSlotApplied() const { return bInitialSelectedHotbarIndexSlotApplied; };

	UFUNCTION(BlueprintPure, Category="CommonInventorySystem")
	int32 GetCurrentSelectedSlotIndex() const { return CurrentSelectedSlotIndex; }
	
	UFUNCTION(BlueprintCallable, Category="CommonInventorySystem")
	void IncrementSelectedHotbarSlot();
	
	UFUNCTION(BlueprintCallable, Category="CommonInventorySystem")
	void DecrementSelectedHotbarSlot();

	UFUNCTION(BlueprintCallable, Category="CommonInventorySystem")
	void FreezeSelectedSlot();
	
	UFUNCTION(BlueprintCallable, Category="CommonInventorySystem")
	void UnFreezeSelectedSlot();

	/** Are we holding an item */
	UFUNCTION(BlueprintPure, Category="CommonInventorySystem")
	bool IsHoldingActorItem() const;
	
	/**
	 *  Adds AdditiveIndex to the current index and wraps it to start or end if it goes out of bounds.
	 *  Internally calls UpdateSelectedHotbarSlot
	 */
	void AdditiveUpdateSelectedHotbarSlot(int32 AdditiveIndex);
	
	void UpdateSelectedHotbarSlot(int32 NewIndex);

protected:
	void DeferredSpawnEquipedItem(const TSoftClassPtr<AActor>& SoftActorClass, float ChangeGameTime, const FCISAttachInventoryItemFragment&
	                              AttachFragment);

	void SpawnActorForCurrentHotbarSelection();
	
	UFUNCTION() void OnInventorySlotUpdatedCallback(UCISInventorySlot* Slot, const FCISInventorySlotUpdateInfo& UpdateInfo);
};
