// By hzFishy - 2025 - Do whatever you want with it.

#pragma once

#include "GameplayTagContainer.h"
#include "CISInventoryDeveloperSettings.generated.h"
class UCISInventorySlot;


/**
 * 
 */
UCLASS(Config=Game, DefaultConfig, DisplayName="Common Inventory System")
class COMMONINVENTORYSYSTEM_API UCISInventoryDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UCISInventoryDeveloperSettings();
	virtual FName GetCategoryName() const override { return "Plugins"; };


	UPROPERTY(Config, EditAnywhere, meta=(Categories="Inventory.Categories"))
	FGameplayTag DefaultInventoryCategoryTag;
	
	UPROPERTY(Config, EditAnywhere, meta=(Categories="Inventory.Categories"))
	FGameplayTag HotbarInventoryCategoryTag;
};
