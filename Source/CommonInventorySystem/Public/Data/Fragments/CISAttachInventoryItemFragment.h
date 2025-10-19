// By hzFishy - 2025 - Do whatever you want with it.

#pragma once

#include "CISBaseInventoryItemFragment.h"
#include "CISAttachInventoryItemFragment.generated.h"

USTRUCT(BlueprintType, DisplayName="Attach Inventory Item Fragment")
struct COMMONINVENTORYSYSTEM_API FCISAttachInventoryItemFragment : public FCISBaseInventoryItemFragment
{
	GENERATED_BODY()
	
	FCISAttachInventoryItemFragment();

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName Socket;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FTransform RelativeSocketTransform;
};
