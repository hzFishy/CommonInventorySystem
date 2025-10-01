// By hzFishy - 2025 - Do whatever you want with it.


#include "Core/CISInventorySlot.h"
#include "Asserts/FUAsserts.h"
#include "Core/CISInventoryItem.h"


FCISInventorySlotUpdateInfo::FCISInventorySlotUpdateInfo() {}

	
	/*----------------------------------------------------------------------------
		Defaults
	----------------------------------------------------------------------------*/
UCISInventorySlot::UCISInventorySlot(): SlotIndex(-1)
{
	
}

	
	/*----------------------------------------------------------------------------
		Core
	----------------------------------------------------------------------------*/
void UCISInventorySlot::InitSlot(FGameplayTag InSlotCategory, int32 InSlotIndex)
{
	SlotCategory = InSlotCategory;
	SlotIndex = InSlotIndex;
}

void UCISInventorySlot::AddItem(UCISInventoryItem* NewItem, bool bCallUpdate)
{
#if WITH_EDITOR
	if (!Items.IsEmpty())
	{
		FU_ENSURE_MSG(NewItem->GetTag() == Items[0]->GetTag(),
			"Adding item with not matching tag (NewItem: %s, Items[0]: %s).",
			*NewItem->GetTag().ToString(),
			*Items[0]->GetTag().ToString()
		);
	}
#endif
	
	if (NewItem->GetOuter() != this)
	{
		NewItem->Rename(nullptr, this);
	}
	
	Items.Emplace(NewItem);

	if (bCallUpdate)
	{
		CallUpdate();
	}
}

void UCISInventorySlot::AddItems(const TArray<UCISInventoryItem*>& NewItems, bool bCallUpdate)
{
	for (auto& NewItem : NewItems)
	{
		AddItem(NewItem, false);
	}

	if (bCallUpdate)
	{
		CallUpdate();
	}
}

int32 UCISInventorySlot::RemoveAmount(int32 AmountToRemove, bool bCallUpdate)
{
	int32 RealRemovingAmount =  GetItemCount() < AmountToRemove ? GetItemCount() : AmountToRemove;

	// items should be GC'ed if not moved to another slot since the only hard ref should be in the slot
	Items.SetNum(Items.Num() - RealRemovingAmount);
	
	if (bCallUpdate)
	{
		CallUpdate();
	}

	return RealRemovingAmount;
}

void UCISInventorySlot::ClearAllItems(bool bCallUpdate)
{
	// items should be GC'ed if not moved to another slot since the only hard ref should be in the slot
	Items.Empty();
	
	if (bCallUpdate)
	{
		CallUpdate();
	}
}

void UCISInventorySlot::CallUpdate()
{
	OnSlotUpdatedDelegate.Broadcast(this, FCISInventorySlotUpdateInfo());
}

FGameplayTag UCISInventorySlot::GetRepresentedItemTag() const
{
	if (!IsEmpty())
	{
		return Items[0]->GetTag();
	}
	return FGameplayTag();
}

const TArray<UCISInventoryItem*>& UCISInventorySlot::GetItems()
{
	return ObjectPtrDecay(Items);
}

TArray<UCISInventoryItem*> UCISInventorySlot::GetItemsCopy()
{
	return ObjectPtrDecay(Items);
}

UCISInventoryItem* UCISInventorySlot::GetRepresentedItem()
{
	if (!IsEmpty())
	{
		return Items[0].Get();
	}
	
	return nullptr;
}
