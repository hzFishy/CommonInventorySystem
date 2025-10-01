// By hzFishy - 2025 - Do whatever you want with it.


#include "Data/CTTypes.h"


	
	/*----------------------------------------------------------------------------
		Craft Query Input Data
	----------------------------------------------------------------------------*/
FCTItemProviderCraftQueryInputDataEntry::FCTItemProviderCraftQueryInputDataEntry():
	Amount(0)
{}

FCTItemProviderCraftQueryInputData::FCTItemProviderCraftQueryInputData() {}

FCTItemProviderCraftQueryOutputData::FCTItemProviderCraftQueryOutputData():
	Amount(0)
{}

FCTItemProviderCraftQuery::FCTItemProviderCraftQuery() {}

	
	/*----------------------------------------------------------------------------
		Item Search Query
	----------------------------------------------------------------------------*/
FCTItemProviderItemSearchQueryResultItemSlot::FCTItemProviderItemSearchQueryResultItemSlot():
	SlotIndex(-1),
	FoundItemAmount(0)
{}

FCTItemProviderItemSearchQueryResultItem::FCTItemProviderItemSearchQueryResultItem()
{}

FCTItemProviderItemSearchQueryResult::FCTItemProviderItemSearchQueryResult() {}

bool FCTItemProviderItemSearchQueryResult::FoundAllItems() const
{
	for (auto& Result : Results)
	{
		/*if (!Result.bHasEnoughAmount)
		{
			return false;
		}*/
	}
	unimplemented();
	return true;
}

FCTItemProviderItemSearchQueryItem::FCTItemProviderItemSearchQueryItem():
	Amount(0)
{}

FCTItemProviderItemSearchQueryItem::FCTItemProviderItemSearchQueryItem(FGameplayTag InItemTag, int32 InAmount):
	ItemTag(InItemTag),
	Amount(InAmount)
{}

FCTItemProviderItemSearchQuery::FCTItemProviderItemSearchQuery() {}

FCTItemProviderItemSearchQuery::FCTItemProviderItemSearchQuery(const FCTItemProviderCraftQuery& CraftQuery)
{
	// use input items to build the items we need to find
	for (auto& CraftInputEntry : CraftQuery.InputData.Entries)
	{
		Items.Emplace(FCTItemProviderItemSearchQueryItem(CraftInputEntry.ItemTag, CraftInputEntry.Amount));
	}
}

void FCTItemProviderItemSearchQuery::GetQuickSearchMap(TMap<FGameplayTag, const FCTItemProviderItemSearchQueryItem>& OutMap) const
{
	OutMap.Reserve(Items.Num());
	
	for (auto& Item : Items)
	{
		OutMap.Add(Item.ItemTag, Item);
	}
}
