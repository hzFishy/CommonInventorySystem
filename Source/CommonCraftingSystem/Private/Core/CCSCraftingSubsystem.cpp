// By hzFishy - 2025 - Do whatever you want with it.


#include "Core/CCSCraftingSubsystem.h"
#include "Data/Core/CCSCraftTypes.h"
#include "Engine/AssetManager.h"

	
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
