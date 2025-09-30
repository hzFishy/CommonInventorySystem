// By hzFishy - 2025 - Do whatever you want with it.

#pragma once

#include "CoreMinimal.h"
#include "CISInventoryPresetDefinition.h"
#include "CISPlayerInventoryPresetDefinition.generated.h"


/**
 * Subclass for the player.
 * Holds data related to the hotbar.
 */
UCLASS(ClassGroup=CommonInventorySystem, DisplayName="Player Inventory Preset Definition")
class COMMONINVENTORYSYSTEM_API UCISPlayerInventoryPresetDefinition : public UCISInventoryPresetDefinition
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	int32 HotbarSize;
};
