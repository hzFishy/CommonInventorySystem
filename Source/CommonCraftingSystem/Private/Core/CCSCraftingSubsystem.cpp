// By hzFishy - 2025 - Do whatever you want with it.


#include "Core/CCSCraftingSubsystem.h"

#include "Asserts/FUAsserts.h"
#include "Data/Core/CCSCraftTypes.h"
#include "Engine/AssetManager.h"
#include "Interfaces/CTItemProviderInterface.h"

	
/*----------------------------------------------------------------------------
		Defaults
	----------------------------------------------------------------------------*/
UCCSCraftingSubsystem::UCCSCraftingSubsystem() {}

void UCCSCraftingSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UCCSCraftingSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

	
	/*----------------------------------------------------------------------------
		Core
	----------------------------------------------------------------------------*/
void UCCSCraftingSubsystem::AsyncLoadAllCraftingRecipes()
{
	if (auto* AM = UAssetManager::GetIfInitialized())
	{
		TArray<FName> Bundles;
		AsyncLoadAllCraftingRecipesHandle = AM->LoadPrimaryAssetsWithType(
			FPrimaryAssetType(CCS::Core::NAME_CraftingRecipeDefinitionType),
			Bundles,
			FStreamableDelegate::CreateWeakLambda(this, [this]()
			{
				FCCSCraftingRecipesSearchResult Result;
				
				TArray<UCCSCraftingRecipeDefinition*> LoadedAssets;
				AsyncLoadAllCraftingRecipesHandle->GetLoadedAssets<UCCSCraftingRecipeDefinition>(LoadedAssets);
				for (auto& Asset : LoadedAssets)
				{
					Result.Entries.Emplace(Asset);
				}
				
				OnAllCraftingRecipesLoadedDelegate.Broadcast(Result);
				AsyncLoadAllCraftingRecipesHandle->ReleaseHandle();
			})
		);
	}
}

bool UCCSCraftingSubsystem::RequestCraftingRecipe(UObject* ItemProvider, const FCCSCraftingRecipeInput& Input,
	const FCCSCraftingRecipeOutput& Output)
{
	auto* ItemProviderInterface = Cast<ICTItemProviderInterface>(ItemProvider);
	if (!FU_ENSURE(ItemProviderInterface)) { return false; }

	FCTItemProviderCraftQuery CraftQuery;

	// input
	{
		for (auto& InputEntry : Input.Entries)
		{
			FCTItemProviderCraftQueryInputDataEntry Entry;
			Entry.ItemTag = InputEntry.ItemTag;
			Entry.Amount = InputEntry.Amount;
			CraftQuery.InputData.Entries.Emplace(Entry);
		}
	}
	
	// output
	{
		CraftQuery.OutputData.ItemTag = Output.Entry.ItemTag;
		CraftQuery.OutputData.Amount = Output.Entry.Amount;
	}
	
	return ItemProviderInterface->CraftRecipe(CraftQuery);
}
