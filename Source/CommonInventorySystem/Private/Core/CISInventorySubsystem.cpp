// By hzFishy - 2025 - Do whatever you want with it.


#include "Core/CISInventorySubsystem.h"
#include "Data/CISInventoryTypes.h"
#include "Data/Definitions/CISInventoryItemDefinition.h"
#include "Engine/AssetManager.h"

	
	/*----------------------------------------------------------------------------
		Defaults
	----------------------------------------------------------------------------*/
UCISInventorySubsystem::UCISInventorySubsystem() {}

void UCISInventorySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UCISInventorySubsystem::Deinitialize()
{
	Super::Deinitialize();
}

	
	/*----------------------------------------------------------------------------
		Core
	----------------------------------------------------------------------------*/
void UCISInventorySubsystem::AsyncLoadItemDefinitionsFromItemTags(FGameplayTagContainer ItemTags)
{
	if (auto* AM = UAssetManager::GetIfInitialized())
	{
		TArray<FPrimaryAssetId> AssetIds;
		for (auto& Tag : ItemTags)
		{
			AssetIds.Emplace(FPrimaryAssetId(CIS::Core::NAME_InventoryItemDefinitionType, FName(Tag.ToString())));
		}

		TArray<FName> Bundles;
		auto LoadHandle = AM->PreloadPrimaryAssets(AssetIds, Bundles, false);

		if (LoadHandle.IsValid())
		{
			if (!LoadHandle->HasLoadCompleted())
			{
				LoadHandle->BindCompleteDelegate(FStreamableDelegate::CreateWeakLambda(this, [this, LoadHandle]()
				{
					TArray<UCISInventoryItemDefinition*> InventoryItemDefs;
					LoadHandle->GetLoadedAssets<UCISInventoryItemDefinition>(InventoryItemDefs);
					OnAsyncLoadItemDefinitionsFromItemTagsFinishedDelegate.Broadcast(InventoryItemDefs);
				}));
				return;
			}
		}

		// Either load already succeeded, or it failed
		
		TArray<UCISInventoryItemDefinition*> LoadedItemDefs;
		for (auto& AssetId : AssetIds)
		{
			auto* LiveAsset = AM->GetPrimaryAssetObject<UCISInventoryItemDefinition>(AssetId);
			if (IsValid(LiveAsset))
			{
				LoadedItemDefs.Emplace(LiveAsset);
			}
		}
		OnAsyncLoadItemDefinitionsFromItemTagsFinishedDelegate.Broadcast(LoadedItemDefs);
	}
}
