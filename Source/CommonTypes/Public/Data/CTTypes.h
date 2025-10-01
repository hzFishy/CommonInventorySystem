// By hzFishy - 2025 - Do whatever you want with it.

#pragma once

#include "GameplayTagContainer.h"
// #include "CTTypes.generated.h"


	
	/*----------------------------------------------------------------------------
		Craft Query Input Data
	----------------------------------------------------------------------------*/
struct COMMONTYPES_API FCTItemProviderCraftQueryInputDataEntry
{
	FCTItemProviderCraftQueryInputDataEntry();

	FGameplayTag ItemTag;
	int32 Amount;
};

struct COMMONTYPES_API FCTItemProviderCraftQueryInputData
{
	FCTItemProviderCraftQueryInputData();

	TArray<FCTItemProviderCraftQueryInputDataEntry> Entries;
};

struct COMMONTYPES_API FCTItemProviderCraftQueryOutputData
{
	FCTItemProviderCraftQueryOutputData();

	FGameplayTag ItemTag;
	int32 Amount;
};

struct COMMONTYPES_API FCTItemProviderCraftQuery
{
	FCTItemProviderCraftQuery();

	/** The items to consume */
	FCTItemProviderCraftQueryInputData InputData;
	
	/** The item to produce */
	FCTItemProviderCraftQueryOutputData OutputData;
};

	
	/*----------------------------------------------------------------------------
		Item Search Query
	----------------------------------------------------------------------------*/
struct COMMONTYPES_API FCTItemProviderItemSearchQueryResultItemSlot
{
	FCTItemProviderItemSearchQueryResultItemSlot();

	FGameplayTag SlotCategoryTag;
	int32 SlotIndex;
	int32 FoundItemAmount;
};

struct COMMONTYPES_API FCTItemProviderItemSearchQueryResultItem
{
	FCTItemProviderItemSearchQueryResultItem();
	
	/**
	 * Slots which had the item.
	 */
	TArray<FCTItemProviderItemSearchQueryResultItemSlot> FoundSlots;
	
	int32 GetTotalFoundAmount() const;
};

struct COMMONTYPES_API FCTItemProviderItemSearchQueryResult
{
	FCTItemProviderItemSearchQueryResult();

	/** Result per searched item */
	TMap<FGameplayTag, FCTItemProviderItemSearchQueryResultItem> Results;

	bool FoundAllItems(const TMap<FGameplayTag, int32>& AmountMap) const;
};

struct COMMONTYPES_API FCTItemProviderItemSearchQueryItem
{
	FCTItemProviderItemSearchQueryItem();
	FCTItemProviderItemSearchQueryItem(FGameplayTag InItemTag, int32 InAmount);

	FGameplayTag ItemTag;
	int32 Amount;
};

struct COMMONTYPES_API FCTItemProviderItemSearchQuery
{
	FCTItemProviderItemSearchQuery();
	FCTItemProviderItemSearchQuery(const FCTItemProviderCraftQuery& CraftQuery);
	
	/** Items to search */
	TArray<FCTItemProviderItemSearchQueryItem> Items;

	/** Key: ItemTag, Value: LinkedQueryItem */
	void BuildQuickSearchMap(TMap<FGameplayTag, const FCTItemProviderItemSearchQueryItem>& OutMap) const;

	TMap<FGameplayTag, int32> BuildAmountMap() const;
};
