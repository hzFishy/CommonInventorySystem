// By hzFishy - 2025 - Do whatever you want with it.


#include "Core/CMCommonMergedLibrary.h"

#include "Asserts/FUAsserts.h"
#include "Data/Definitions/CISInventoryItemDefinition.h"
#include "Data/Fragments/CISUIInventoryItemFragment.h"


FCCSCraftingWidgetRecipeData UCMCommonMergedLibrary::CollectCraftingWidgetRecipeDataFromInventoryItems(
	UCISInventoryItemDefinition* OutputItemDefinition,
	const TArray<UCISInventoryItemDefinition*>& InputItemDefinitions,
	const FCCSCraftingRecipeOutputEntry& CraftingRecipeOutputEntry,
	const TArray<FCCSCraftingRecipeInputEntry>& CraftingRecipeInputEntries)
{
	FU_ENSURE(InputItemDefinitions.Num() == CraftingRecipeInputEntries.Num());
	
	FCCSCraftingWidgetRecipeData RecipeData;

	if (auto* OutputUIFragment = OutputItemDefinition->GetFragmentFromType<FCISUIInventoryItemFragment>())
	{
		RecipeData.OutputData.DisplayName = OutputUIFragment->DisplayName;
		RecipeData.OutputData.Tooltip = OutputUIFragment->Tooltip;
		RecipeData.OutputData.SoftIconTexture = OutputUIFragment->Icon;
		RecipeData.OutputData.Amount = CraftingRecipeOutputEntry.Amount;
	}

	for (int i = 0; i < InputItemDefinitions.Num(); ++i)
	{
		auto& InputItemDefinition = InputItemDefinitions[i];
		auto& CraftingRecipeInputEntry = CraftingRecipeInputEntries[i];
		
		FCCSCraftingWidgetRecipeInputData InputDataEntry;
		if (auto* InputUIFragment = InputItemDefinition->GetFragmentFromType<FCISUIInventoryItemFragment>())
		{
			InputDataEntry.DisplayName = InputUIFragment->DisplayName;
			InputDataEntry.SoftIconTexture = InputUIFragment->Icon;
			InputDataEntry.Amount = CraftingRecipeInputEntry.Amount;
		}
		RecipeData.InputData.Emplace(InputDataEntry);
	}

	return RecipeData;
}
