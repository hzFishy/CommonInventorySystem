// By hzFishy - 2025 - Do whatever you want with it.

#pragma once

#include "CoreMinimal.h"
#include "CISBaseInventoryComponent.h"
#include "Core/CISInventorySlot.h"
#include "Data/CISInventoryTypes.h"
#include "CISPlayerInventoryComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCISOnPlayerInventorySelectedHotbarSlotChangedSignature,
	UCISPlayerInventoryComponent*, PlayerInventoryComponent,
	const FCISHotbarChangedEvent&, HotbarChangedEvent
);


UCLASS(ClassGroup=(CommonInventorySystem), DisplayName="Player Inventory Component", meta=(BlueprintSpawnableComponent))
class COMMONINVENTORYSYSTEM_API UCISPlayerInventoryComponent : public UCISBaseInventoryComponent
{
	GENERATED_BODY()

	
	/*----------------------------------------------------------------------------
		Properties
	----------------------------------------------------------------------------*/
public:
	UPROPERTY(BlueprintAssignable, DisplayName="On Selected Hotbar Slot Changed")
	FCISOnPlayerInventorySelectedHotbarSlotChangedSignature OnSelectedHotbarSlotChangedDelegate;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="CommonInventorySystem")
	TOptional<int32> InitialSelectedHotbarIndexSlot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="CommonInventorySystem")
	FName HotbarEquipSocket;
	
	TWeakObjectPtr<UPrimitiveComponent> HotbarEquipComponent;
	bool bInitialSelectedHotbarIndexSlotApplied;
	int32 HotbarMaxIndex;
	int32 CurrentSelectedSlotIndex;
	int32 FreezeSelectedSlotCount;
	FCISInventoryHotbarSelection CurrentHotbarSelection;
	
	
	/*----------------------------------------------------------------------------
		Defaults
	----------------------------------------------------------------------------*/
public:
	UCISPlayerInventoryComponent();

	
	/*----------------------------------------------------------------------------
		Init
	----------------------------------------------------------------------------*/
protected:
	virtual void OnInventoryItemDefinitionsLoaded(TSoftClassPtr<UCISInventorySlot> SoftInventorySlotClass,
		TArray<FCISInventorySlotDefinition> SlotDefinitions) override;

	
	/*----------------------------------------------------------------------------
		Hotbar
	----------------------------------------------------------------------------*/
public:
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

	void DeferredSpawnEquipedItem(const TSoftClassPtr<AActor>& SoftActorClass, float ChangeGameTime);
	
protected:
	void UpdateSelectedHotbarSlot(int32 AdditiveIndex);

	void SpawnActorForCurrentHotbarSelection();
	
	UFUNCTION() void OnInventorySlotUpdatedCallback(UCISInventorySlot* Slot, const FCISInventorySlotUpdateInfo& UpdateInfo);
};
