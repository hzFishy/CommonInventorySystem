// By hzFishy - 2025 - Do whatever you want with it.

#pragma once

#include "Data/Core/CCSCraftTypes.h"
#include "Engine/DataAsset.h"
#include "CCSCraftingRecipeDefinition.generated.h"


/**
 * Defines the inputs and outputs of a crafting recipe.
 */
UCLASS(ClassGroup=CommonCraftingSystem, DisplayName="Crafting Recipe Definition")
class COMMONCRAFTINGSYSTEM_API UCCSCraftingRecipeDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UCCSCraftingRecipeDefinition();
	
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ShowOnlyInnerProperties))
	FCCSCraftingRecipe CraftingRecipe;
};
