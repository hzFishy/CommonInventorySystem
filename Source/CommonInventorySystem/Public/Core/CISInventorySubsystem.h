// By hzFishy - 2025 - Do whatever you want with it.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/StreamableManager.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CISInventorySubsystem.generated.h"
class UCISInventoryItemDefinition;


DECLARE_MULTICAST_DELEGATE_OneParam(FCCSOnAsyncLoadItemDefinitionsFromItemTagsFinishedSignature,
	const TArray<UCISInventoryItemDefinition*>& /* LoadedItemDefinitions */
);


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
public:
	FCCSOnAsyncLoadItemDefinitionsFromItemTagsFinishedSignature OnAsyncLoadItemDefinitionsFromItemTagsFinishedDelegate;

protected:
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
	void AsyncLoadItemDefinitionsFromItemTags(FGameplayTagContainer ItemTags);
};
