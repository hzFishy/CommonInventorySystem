// By hzFishy - 2025 - Do whatever you want with it.

#pragma once

#include "UObject/Interface.h"
#include "Data/CTTypes.h"
#include "CTItemProviderInterface.generated.h"


UINTERFACE()
class UCTItemProviderInterface : public UInterface
{
	GENERATED_BODY()
};


/**
 * Interface used on objects which contains items.
 * For example, the player inventory.
 */
class COMMONTYPES_API ICTItemProviderInterface
{
	GENERATED_BODY()

public:
	virtual bool SearchItems(const FCTItemProviderItemSearchQuery& Query, FCTItemProviderItemSearchQueryResult& QueryResult) = 0;

	
	/**
	 *  Craft the recipe.
	 *  @returns true if the item was crafted.
	 */
	virtual bool CraftRecipe(const FCTItemProviderCraftQuery& CraftQuery) = 0;
};
