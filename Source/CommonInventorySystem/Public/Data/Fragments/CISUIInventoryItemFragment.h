// By hzFishy - 2025 - Do whatever you want with it.

#pragma once

#include "CISBaseInventoryItemFragment.h"
#include "CISUIInventoryItemFragment.generated.h"


USTRUCT(BlueprintType, DisplayName="UI Inventory Item Fragment")
struct COMMONINVENTORYSYSTEM_API FCISUIInventoryItemFragment : public FCISBaseInventoryItemFragment
{
	GENERATED_BODY()

	FCISUIInventoryItemFragment();

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> Icon;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText DisplayName;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(MultiLine))
	FText Tooltip;
};
