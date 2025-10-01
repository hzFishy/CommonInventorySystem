// By hzFishy - 2025 - Do whatever you want with it.


#include "Data/Core/CCSCraftTypes.h"

	
	/*----------------------------------------------------------------------------
		Crafting Input
	----------------------------------------------------------------------------*/
FCCSCraftingRecipeInputEntry::FCCSCraftingRecipeInputEntry():
	Amount(1)
{}

FCCSCraftingRecipeInputEntry::FCCSCraftingRecipeInputEntry(const FCCSCraftingWidgetRecipeInputData& InputData)
{
	ItemTag = InputData.ItemTag;
	Amount = InputData.Amount;
}


FCCSCraftingRecipeInput::FCCSCraftingRecipeInput() {}


	
	/*----------------------------------------------------------------------------
		Crafting Output
	----------------------------------------------------------------------------*/
FCCSCraftingRecipeOutputEntry::FCCSCraftingRecipeOutputEntry(): Amount(1)
{}

FCCSCraftingRecipeOutput::FCCSCraftingRecipeOutput() {}

FCCSCraftingRecipeOutput::FCCSCraftingRecipeOutput(const FCCSCraftingWidgetRecipeOutputData& OutputData)
{
	Entry.ItemTag = OutputData.ItemTag;
	Entry.Amount = OutputData.Amount;
}

	
	/*----------------------------------------------------------------------------
		Crafting Recipes Search
	----------------------------------------------------------------------------*/
FCCSCraftingRecipesSearchResultEntry::FCCSCraftingRecipesSearchResultEntry()
{}

FCCSCraftingRecipesSearchResultEntry::FCCSCraftingRecipesSearchResultEntry(UCCSCraftingRecipeDefinition* InAsset): LoadedCraftingRecipe(InAsset)
{}

FCCSCraftingRecipesSearchResult::FCCSCraftingRecipesSearchResult() {}

	
	/*----------------------------------------------------------------------------
		UI Helpers
	----------------------------------------------------------------------------*/
FCCSCraftingWidgetRecipeInputData::FCCSCraftingWidgetRecipeInputData():
	Amount(0)
{}

FCCSCraftingWidgetRecipeOutputData::FCCSCraftingWidgetRecipeOutputData():
	Amount(0)
{}

FCCSCraftingWidgetRecipeData::FCCSCraftingWidgetRecipeData() {}
