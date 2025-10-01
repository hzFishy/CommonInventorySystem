// By hzFishy - 2025 - Do whatever you want with it.

#pragma once

#include "CoreMinimal.h"
#include "Data/Core/CCSCraftTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CMCommonMergedLibrary.generated.h"


class UCISInventoryItemDefinition;
/**
 * 
 */
UCLASS(DisplayName="Common Merged Library")
class COMMONMERGE_API UCMCommonMergedLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="CommonMerged")
	static FCCSCraftingWidgetRecipeData CollectCraftingWidgetRecipeDataFromInventoryItems(
		UCISInventoryItemDefinition* OutputItemDefinition,
		const TArray<UCISInventoryItemDefinition*>& InputItemDefinitions,
		const FCCSCraftingRecipeOutputEntry& CraftingRecipeOutputEntry,
		const TArray<FCCSCraftingRecipeInputEntry>& CraftingRecipeInputEntries);

	UFUNCTION(BlueprintPure, Category="CommonMerged")
	static FCCSCraftingRecipeOutput CraftingWidgetRecipeOutputToCraftingRecipeOutput(const FCCSCraftingWidgetRecipeOutputData& OutputData);
	
	UFUNCTION(BlueprintPure, Category="CommonMerged")
	static FCCSCraftingRecipeInput CraftingWidgetRecipeInputToCraftingRecipeOutput(const TArray<FCCSCraftingWidgetRecipeInputData>& InputData);
};
