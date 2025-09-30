// By hzFishy - 2025 - Do whatever you want with it.

#pragma once
#include "GameplayTagContainer.h"

#include "CCSCraftTypes.generated.h"
class UCCSCraftingRecipeDefinition;

namespace CCS::Core
{
	inline FName NAME_CraftingRecipeDefinitionType = "CraftingRecipeDefinition";
}

	
	/*----------------------------------------------------------------------------
		Crafting Input
	----------------------------------------------------------------------------*/
USTRUCT(BlueprintType, DisplayName="Crafting Recipe Input Entry", BlueprintType)
struct COMMONCRAFTINGSYSTEM_API FCCSCraftingRecipeInputEntry
{
	GENERATED_BODY()

	FCCSCraftingRecipeInputEntry();

	
	UPROPERTY(EditAnywhere, meta=(Categories="Inventory.Items"), BlueprintReadOnly)
	FGameplayTag ItemTag;
	
	UPROPERTY(EditAnywhere, meta=(UIMin=0), BlueprintReadOnly)
	int32 Amount;
};


USTRUCT(BlueprintType, DisplayName="Crafting Recipe Input", BlueprintType)
struct COMMONCRAFTINGSYSTEM_API FCCSCraftingRecipeInput
{
	GENERATED_BODY()

	FCCSCraftingRecipeInput();

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FCCSCraftingRecipeInputEntry> Entries;
};

	
	/*----------------------------------------------------------------------------
		Crafting Output
	----------------------------------------------------------------------------*/
USTRUCT(BlueprintType, DisplayName="Crafting Recipe Output Entry", BlueprintType)
struct COMMONCRAFTINGSYSTEM_API FCCSCraftingRecipeOutputEntry
{
	GENERATED_BODY()

	FCCSCraftingRecipeOutputEntry();

	
	UPROPERTY(EditAnywhere, meta=(Categories="Inventory.Items"), BlueprintReadOnly)
	FGameplayTag ItemTag;
	
	UPROPERTY(EditAnywhere, meta=(UIMin=0), BlueprintReadOnly)
	int32 Amount;
};


USTRUCT(BlueprintType, DisplayName="Crafting Recipe Output", BlueprintType)
struct COMMONCRAFTINGSYSTEM_API FCCSCraftingRecipeOutput
{
	GENERATED_BODY()

	FCCSCraftingRecipeOutput();

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FCCSCraftingRecipeOutputEntry Entry;
};

	
	/*----------------------------------------------------------------------------
		Crafting Recipes Search
	----------------------------------------------------------------------------*/
USTRUCT(BlueprintType, DisplayName="Crafting Recipes Search Result Entry")
struct FCCSCraftingRecipesSearchResultEntry
{
	GENERATED_BODY()

	FCCSCraftingRecipesSearchResultEntry();
	FCCSCraftingRecipesSearchResultEntry(UCCSCraftingRecipeDefinition* InAsset);

	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<UCCSCraftingRecipeDefinition> LoadedCraftingRecipe;
};

USTRUCT(BlueprintType, DisplayName="Crafting Recipes Search Result")
struct FCCSCraftingRecipesSearchResult
{
	GENERATED_BODY()

	FCCSCraftingRecipesSearchResult();

	UPROPERTY(BlueprintReadOnly)
	TArray<FCCSCraftingRecipesSearchResultEntry> Entries;
};
