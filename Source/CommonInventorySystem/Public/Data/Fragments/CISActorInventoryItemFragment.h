// By hzFishy - 2025 - Do whatever you want with it.

#pragma once

#include "CISBaseInventoryItemFragment.h"
#include "CISActorInventoryItemFragment.generated.h"


USTRUCT(BlueprintType, DisplayName="Actor Inventory Item Fragment")
struct COMMONINVENTORYSYSTEM_API FCISActorInventoryItemFragment : public FCISBaseInventoryItemFragment
{
	GENERATED_BODY()
	
	FCISActorInventoryItemFragment();

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftClassPtr<AActor> SoftActorClass;
};
