// By hzFishy - 2025 - Do whatever you want with it.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Data/CISInventoryTypes.h"
#include "Data/Definitions/CISInventoryItemDefinition.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CISInventorySubsystem.generated.h"
class UCISInventoryItemDefinition;


/**
 * 
 */
UCLASS(DisplayName = "Inventory Subsystem")
class COMMONINVENTORYSYSTEM_API UCISInventorySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	
	/*----------------------------------------------------------------------------
		Properties
	----------------------------------------------------------------------------*/
protected:
	uint32 HighestAsyncRequestId;

	
	/*----------------------------------------------------------------------------
		Defaults
	----------------------------------------------------------------------------*/
public:
	UCISInventorySubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	virtual void Deinitialize() override;

	
	/*----------------------------------------------------------------------------
		Core
	----------------------------------------------------------------------------*/
public:
	// delegate needs to impl "const TArray<UCISInventoryItemDefinition*>& LoadedItemDefinitions" as first param
	template<typename Func>
	void AsyncLoadItemDefinitionsFromItemTags(FGameplayTagContainer ItemTags, Func OnComplete)
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
					LoadHandle->BindCompleteDelegate(FStreamableDelegate::CreateWeakLambda(this, [this, LoadHandle, OnComplete]()
					{
						TArray<UCISInventoryItemDefinition*> InventoryItemDefs;
						LoadHandle->GetLoadedAssets<UCISInventoryItemDefinition>(InventoryItemDefs);
						OnComplete(InventoryItemDefs);
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
			OnComplete(LoadedItemDefs);
		}
	}

	FCISAsyncInventoryItemDefinitionRequest MakeAsyncItemDefinitionRequest();
};
