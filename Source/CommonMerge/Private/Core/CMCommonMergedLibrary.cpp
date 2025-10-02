// By hzFishy - 2025 - Do whatever you want with it.


#include "Core/CMCommonMergedLibrary.h"

#include "Asserts/FUAsserts.h"
#include "Data/Definitions/CISInventoryItemDefinition.h"
#include "Data/Fragments/CISUIInventoryItemFragment.h"


FCCSCraftingWidgetRecipeData UCMCommonMergedLibrary::CollectCraftingWidgetRecipeDataFromInventoryItems(
	UCISInventoryItemDefinition* OutputItemDefinition,
	const TArray<UCISInventoryItemDefinition*>& InputItemDefinitions,
	const FCCSCraftingRecipe& CraftingRecipe)
{
	if (!FU_ENSURE_VALID(OutputItemDefinition))
	{
		return FCCSCraftingWidgetRecipeData();
	}
	if (!FU_ENSURE(InputItemDefinitions.Num() == CraftingRecipe.CraftingRecipeInput.Entries.Num()))
	{
		return FCCSCraftingWidgetRecipeData();
	}
	
	FCCSCraftingWidgetRecipeData RecipeData;

	if (auto* OutputUIFragment = OutputItemDefinition->GetFragmentFromType<FCISUIInventoryItemFragment>())
	{
		RecipeData.OutputData.DisplayName = OutputUIFragment->DisplayName;
		RecipeData.OutputData.Tooltip = OutputUIFragment->Tooltip;
		RecipeData.OutputData.SoftIconTexture = OutputUIFragment->Icon;
		RecipeData.OutputData.Amount = CraftingRecipe.CraftingRecipeOutput.Entry.Amount;
		RecipeData.OutputData.ItemTag = CraftingRecipe.CraftingRecipeOutput.Entry.ItemTag;
	}

	for (int i = 0; i < InputItemDefinitions.Num(); ++i)
	{
		auto& InputItemDefinition = InputItemDefinitions[i];
		auto& CraftingRecipeInputEntry = CraftingRecipe.CraftingRecipeInput.Entries[i];
		
		FCCSCraftingWidgetRecipeInputData InputDataEntry;
		if (auto* InputUIFragment = InputItemDefinition->GetFragmentFromType<FCISUIInventoryItemFragment>())
		{
			InputDataEntry.DisplayName = InputUIFragment->DisplayName;
			InputDataEntry.SoftIconTexture = InputUIFragment->Icon;
			InputDataEntry.Amount = CraftingRecipeInputEntry.Amount;
			InputDataEntry.ItemTag = CraftingRecipeInputEntry.ItemTag;
		}
		RecipeData.InputData.Emplace(InputDataEntry);
	}

	return RecipeData;
}

FCCSCraftingRecipeOutput UCMCommonMergedLibrary::CraftingWidgetRecipeOutputToCraftingRecipeOutput(
	const FCCSCraftingWidgetRecipeOutputData& OutputData)
{
	return FCCSCraftingRecipeOutput(OutputData);
}

FCCSCraftingRecipeInput UCMCommonMergedLibrary::CraftingWidgetRecipeInputToCraftingRecipeOutput(
	const TArray<FCCSCraftingWidgetRecipeInputData>& InputData)
{
	FCCSCraftingRecipeInput Input;

	for (auto& InputDataEntry : InputData)
	{
		Input.Entries.Emplace(InputDataEntry);
	}

	return Input;
}
