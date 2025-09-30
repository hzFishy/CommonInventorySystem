// By hzFishy - 2025 - Do whatever you want with it.


#include "Data/Definitions/CCSCraftingRecipeDefinition.h"


UCCSCraftingRecipeDefinition::UCCSCraftingRecipeDefinition() {}

FPrimaryAssetId UCCSCraftingRecipeDefinition::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(CCS::Core::NAME_CraftingRecipeDefinitionType, GetFName());
}
