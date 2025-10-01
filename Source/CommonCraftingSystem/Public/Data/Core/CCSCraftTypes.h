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
	FCCSCraftingRecipeInputEntry(const FCCSCraftingWidgetRecipeInputData& InputData);

	
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
	FCCSCraftingRecipeOutput(const FCCSCraftingWidgetRecipeOutputData& OutputData);

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FCCSCraftingRecipeOutputEntry Entry;
};

	
	/*----------------------------------------------------------------------------
		Crafting Recipes Search
	----------------------------------------------------------------------------*/
USTRUCT(BlueprintType, DisplayName="Crafting Recipes Search Result Entry")
struct COMMONCRAFTINGSYSTEM_API FCCSCraftingRecipesSearchResultEntry
{
	GENERATED_BODY()

	FCCSCraftingRecipesSearchResultEntry();
	FCCSCraftingRecipesSearchResultEntry(UCCSCraftingRecipeDefinition* InAsset);

	UPROPERTY(BlueprintReadOnly)
	TWeakObjectPtr<UCCSCraftingRecipeDefinition> LoadedCraftingRecipe;
};

USTRUCT(BlueprintType, DisplayName="Crafting Recipes Search Result")
struct COMMONCRAFTINGSYSTEM_API FCCSCraftingRecipesSearchResult
{
	GENERATED_BODY()

	FCCSCraftingRecipesSearchResult();

	UPROPERTY(BlueprintReadOnly)
	TArray<FCCSCraftingRecipesSearchResultEntry> Entries;
};

	
	/*----------------------------------------------------------------------------
		UI Helpers
	----------------------------------------------------------------------------*/
USTRUCT(BlueprintType, Displayname="Crafting Widget Recipe Input Data")
struct COMMONCRAFTINGSYSTEM_API FCCSCraftingWidgetRecipeInputData
{
	GENERATED_BODY()

	FCCSCraftingWidgetRecipeInputData();

	
	UPROPERTY(BlueprintReadOnly)
	FText DisplayName;

	UPROPERTY(BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> SoftIconTexture;
	
	UPROPERTY(BlueprintReadOnly)
	int32 Amount;
	
	UPROPERTY(BlueprintReadOnly)
	FGameplayTag ItemTag;
};
USTRUCT(BlueprintType, Displayname="Crafting Widget Recipe Output Data")
struct COMMONCRAFTINGSYSTEM_API FCCSCraftingWidgetRecipeOutputData
{
	GENERATED_BODY()

	FCCSCraftingWidgetRecipeOutputData();

	
	UPROPERTY(BlueprintReadOnly)
	FText DisplayName;

	UPROPERTY(BlueprintReadOnly)
	FText Tooltip;
	
	UPROPERTY(BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> SoftIconTexture;
	
	UPROPERTY(BlueprintReadOnly)
	int32 Amount;
	
	UPROPERTY(BlueprintReadOnly)
	FGameplayTag ItemTag;
};

USTRUCT(BlueprintType, Displayname="Crafting Widget Recipe Data")
struct COMMONCRAFTINGSYSTEM_API FCCSCraftingWidgetRecipeData
{
	GENERATED_BODY()

	FCCSCraftingWidgetRecipeData();
	

	UPROPERTY(BlueprintReadOnly)
	FCCSCraftingWidgetRecipeOutputData OutputData;

	UPROPERTY(BlueprintReadOnly)
	TArray<FCCSCraftingWidgetRecipeInputData> InputData;
};
