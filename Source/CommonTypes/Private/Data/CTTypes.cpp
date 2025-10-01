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

int32 FCTItemProviderItemSearchQueryResultItem::GetTotalFoundAmount() const
{
	int32 Amount = 0;
	for (auto& FoundSlot : FoundSlots)
	{
		Amount += FoundSlot.FoundItemAmount;
	}
	return Amount;
}

FCTItemProviderItemSearchQueryResult::FCTItemProviderItemSearchQueryResult() {}

bool FCTItemProviderItemSearchQueryResult::FoundAllItems(const TMap<FGameplayTag, int32>& AmountMap) const
{
	for (auto& AmountData : AmountMap)
	{
		auto& ResultData = Results[AmountData.Key];
		if (ResultData.GetTotalFoundAmount() < AmountData.Value)
		{
			return false;
		}
	}
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

void FCTItemProviderItemSearchQuery::BuildQuickSearchMap(TMap<FGameplayTag, const FCTItemProviderItemSearchQueryItem>& OutMap) const
{
	OutMap.Reserve(Items.Num());
	
	for (auto& Item : Items)
	{
		OutMap.Add(Item.ItemTag, Item);
	}
}

TMap<FGameplayTag, int32> FCTItemProviderItemSearchQuery::BuildAmountMap() const
{
	TMap<FGameplayTag, int32> AmountMap;
	for (auto& Item : Items)
	{
		AmountMap.Add(Item.ItemTag, Item.Amount);
	}
	return AmountMap;
}
